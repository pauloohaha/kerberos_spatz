// Copyright 2021 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

// Author: Domenic Wüthrich, ETH Zurich

module spatz_vfu import spatz_pkg::*; (
  input  logic clk_i,
  input  logic rst_ni,
  // Spatz req
  input  spatz_req_t spatz_req_i,
  input  logic       spatz_req_valid_i,
  output logic       spatz_req_ready_o,
  // VFU rsp
  output logic     vfu_rsp_valid_o,
  output vfu_rsp_t vfu_rsp_o,
  // VRF
  output vreg_addr_t       vrf_waddr_o,
  output vreg_data_t       vrf_wdata_o,
  output logic             vrf_we_o,
  output vreg_be_t         vrf_wbe_o,
  input  logic             vrf_wvalid_i,
  output vreg_addr_t [2:0] vrf_raddr_o,
  output logic       [2:0] vrf_re_o,
  input  vreg_data_t [2:0] vrf_rdata_i,
  input  logic       [2:0] vrf_rvalid_i
);

  logic vfu_is_ready;
  logic op1_is_ready, op2_is_ready, op3_is_ready;
  logic operands_ready;
  assign operands_ready = op1_is_ready && op2_is_ready && op3_is_ready;

  logic last_group;
  logic result_written;

  logic new_request;
  assign new_request = spatz_req_valid_i && vfu_is_ready && (spatz_req_i.ex_unit == VFU);

  logic [N_IPU*ELEN-1:0]  operand1, operand2, operand3;
  logic [N_IPU*ELENB-1:0] carry;
  logic [N_IPU*ELENB-1:0] result_be;
  logic [N_IPU*ELEN-1:0]  result;

  ///////////////
  // Spatz Req //
  ///////////////

  spatz_req_t spatz_req_d, spatz_req_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin : proc_spatz_req
    if(~rst_ni) begin
      spatz_req_q <= '0;
    end else begin
      spatz_req_q <= spatz_req_d;
    end
  end

  assign vfu_is_ready = (spatz_req_q.vl == 0) || (last_group && operands_ready && result_written);
  assign spatz_req_ready_o = vfu_is_ready;

  ///////////////////
  // State Handler //
  ///////////////////

  always_comb begin : proc_state_handler
    spatz_req_d = spatz_req_q;

    if (new_request) begin
      spatz_req_d = spatz_req_i;
    end else if (spatz_req_q.vl != 0 && operands_ready && result_written) begin
      // Change number of remaining elements
      if (spatz_req_q.vtype.vsew == rvv_pkg::EW_8) begin
        spatz_req_d.vl = last_group ? 0 : spatz_req_q.vl - N_IPU * 4;
      end else if (spatz_req_q.vtype.vsew == rvv_pkg::EW_16) begin
        spatz_req_d.vl = last_group ? 0 : spatz_req_q.vl - N_IPU * 2;
      end else begin
        spatz_req_d.vl = last_group ? 0 : spatz_req_q.vl - N_IPU;
      end
    end else if (spatz_req_q.vl == 0) begin
      spatz_req_d = '0;
    end
  end : proc_state_handler

  // Determine if we are currently handling the last group
  always_comb begin : proc_last_group
    last_group = 1'b0;

    if (spatz_req_q.vtype.vsew == rvv_pkg::EW_8) begin
        last_group = spatz_req_q.vl <= N_IPU * 4;
      end else if (spatz_req_q.vtype.vsew == rvv_pkg::EW_16) begin
        last_group = spatz_req_q.vl <= N_IPU * 2;
      end else begin
        last_group = spatz_req_q.vl <= N_IPU;
      end
  end

  always_comb begin : proc_vfu_rsp
    vfu_rsp_valid_o = 1'b0;
    vfu_rsp_o       = '0;

    if (last_group && operands_ready && result_written) begin
      vfu_rsp_o.id    = spatz_req_q.id;
      vfu_rsp_valid_o = 1'b1;
    end
  end

  ///////////////////////
  // Operand Requester //
  ///////////////////////

  vreg_addr_t [2:0] vreg_addr_q, vreg_addr_d;
  vreg_be_t         vreg_wbe;
  logic             vreg_we;
  logic [2:0]       vreg_r_req;

  always_ff @(posedge clk_i or negedge rst_ni) begin : proc_vreg_addr_q
    if(~rst_ni) begin
      vreg_addr_q <= 0;
    end else begin
      vreg_addr_q <= vreg_addr_d;
    end
  end

  always_comb begin : proc_vreg_addr
    vreg_addr_d = vreg_addr_q;

    if (new_request) begin
      vreg_addr_d[0] = {spatz_req_i.vs2, $clog2(VELE)'(0)};
      vreg_addr_d[1] = {spatz_req_i.vs1, $clog2(VELE)'(0)};
      vreg_addr_d[2] = {spatz_req_i.vd,  $clog2(VELE)'(0)};
    end else if (spatz_req_d.vl != 0 && operands_ready && result_written) begin
      vreg_addr_d[0] = vreg_addr_q[0] + 1;
      vreg_addr_d[1] = vreg_addr_q[1] + 1;
      vreg_addr_d[2] = vreg_addr_q[2] + 1;
    end else if (spatz_req_d.vl == 0) begin
      vreg_addr_d = '0;
    end
  end

  always_comb begin : proc_op_req
    vreg_r_req = '0;
    vreg_we = '0;
    vreg_wbe = '0;

    if (spatz_req_q.vl != 0) begin
      // Request operands
      vreg_r_req = {spatz_req_q.vd_is_src, spatz_req_q.use_vs1, spatz_req_q.use_vs2};

      // Distribute operands
      if (operands_ready) begin
        vreg_we  = spatz_req_q.use_vd;
        vreg_wbe = '1;

        if (last_group) begin
          automatic logic [N_IPU*4-1:0] base_mask = '1;
          automatic int opa = spatz_req_q.vtype.vsew == rvv_pkg::EW_8  ? N_IPU*4 :
                              spatz_req_q.vtype.vsew == rvv_pkg::EW_16 ? N_IPU*2 : N_IPU;
          automatic int opb = spatz_req_q.vtype.vsew == rvv_pkg::EW_8  ? spatz_req_q.vl[$clog2(N_IPU * 4):0] :
                              spatz_req_q.vtype.vsew == rvv_pkg::EW_16 ? spatz_req_q.vl[$clog2(N_IPU * 2):0] :
                                                                         spatz_req_q.vl[$clog2(N_IPU):0];
          vreg_wbe = base_mask >> opa - opb;
        end
      end
    end
  end : proc_op_req

  assign vrf_raddr_o = vreg_addr_q;
  assign vrf_re_o    = vreg_r_req;
  assign vrf_we_o    = vreg_we;
  assign vrf_wbe_o   = vreg_wbe;
  assign vrf_waddr_o = vreg_addr_q;
  assign vrf_wdata_o = result;

  assign op1_is_ready = spatz_req_q.use_vs1   ? vrf_rvalid_i[1] : 1'b1;
  assign op2_is_ready = spatz_req_q.use_vs2   ? vrf_rvalid_i[0] : 1'b1;
  assign op3_is_ready = spatz_req_q.vd_is_src ? vrf_rvalid_i[2] : 1'b1;
  assign operand1 = spatz_req_q.use_vs1 ? vrf_rdata_i[1] : spatz_req_q.vtype.vsew == rvv_pkg::EW_8 ? {4*N_IPU{spatz_req_q.rs1[7:0]}} : spatz_req_q.vtype.vsew == rvv_pkg::EW_16 ? {2*N_IPU{spatz_req_q.rs1[15:0]}} : {N_IPU{spatz_req_q.rs1}};
  assign operand2 = vrf_rdata_i[0];
  assign operand3 = vrf_rdata_i[2];
  assign result_written = spatz_req_q.use_vd ? vrf_wvalid_i : 1'b1;

  //////////
  // IPUs //
  //////////

  for (genvar i = 0; unsigned'(i) < N_IPU; i++) begin : gen_ipus
    spatz_ipu i_ipu (
      .clk_i      (clk_i),
      .rst_ni     (rst_ni),
      .operation_i(spatz_req_q.op),
      .op_s1_i    (operand1[i*ELEN +: ELEN]),
      .op_s2_i    (operand2[i*ELEN +: ELEN]),
      .op_d_i     (operand3[i*ELEN +: ELEN]),
      .carry_i    ('0),
      .sew_i      (spatz_req_q.vtype.vsew),
      .be_o       (result_be[i*ELENB +: ELENB]),
      .result_o   (result[i*ELEN +: ELEN])
    );
  end : gen_ipus

endmodule : spatz_vfu
