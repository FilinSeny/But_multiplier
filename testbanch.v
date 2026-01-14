module testbanch#(
  parameter m_size   = 4,
  parameter r_size   = 4,
  parameter res_size = m_size + r_size
)();

  reg signed [m_size-1:0]   M;
  reg signed [r_size-1:0]   R;
  wire signed [res_size-1:0] RES;

  But_multiplier exmp(
    .M(M),
    .R(R),
    .RES(RES)
  );

  task test;
    input signed [m_size-1:0]   t_a;
    input signed [r_size-1:0]   t_b;
    input signed [res_size-1:0] t_mul;
    begin
      M = t_a;
      R = t_b;

      #1;

      $display("TEST %0d * %0d = %0d", t_a, t_b, RES);

      if (RES !== t_mul) begin
        $display("FAIL %s: expected %0d got %0d", `__FILE__, t_mul, RES);
        $finish;
      end
    end
  endtask

  initial begin
    $dumpfile("dump.vcd");
    $dumpvars(0, testbanch);
    #1
    test(0, 0, 0);
    #1
    test(1, 2, 2);
    #1
    test(1, -2, -2);
    #1
    test(-1, 2, -2);
    #1
    test(-1, -2, 2);
    #1

    $display("PASS %s", `__FILE__);
    $finish;
  end

endmodule
