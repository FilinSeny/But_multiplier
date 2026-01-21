module  But_multiplier#(
    parameter m_size =      4, ///m_size always greater or equal to r_size
    parameter r_size =      4,

    parameter res_size =    m_size + r_size
)
(
    input wire[m_size - 1:0]    M,
    input wire[r_size - 1:0]    R,
    output wire [res_size - 1: 0]   RES
);


    wire        [r_size:0] zero;
    assign      zero    = 0;
    wire signed [m_size-1:0] M_s;
    assign M_s = $signed(M);      

    wire signed [m_size-1:0] M_neg;
    assign M_neg = -M_s;
    
    wire signed [res_size:0] A = {M_s, zero};
    wire signed [res_size:0] S = {M_neg, zero}; 
    
    wire signed [res_size:0] P = {{(m_size){1'b0}}, R, 1'b0};
     
    wire signed [res_size:0] Pi_res [0:r_size];

    assign Pi_res[0] = P;
    
    genvar i;
    generate
        for (i = 0; i < r_size; i = i + 1) begin : but_stages
            wire signed [res_size:0] P_before_shift;
            
            assign P_before_shift = 
                (Pi_res[i][1:0] == 2'b01) ? Pi_res[i] + A :
                (Pi_res[i][1:0] == 2'b10) ? Pi_res[i] + S :
                Pi_res[i];
            
            assign Pi_res[i+1] = P_before_shift >>> 1;
        end
    endgenerate
    
    assign RES = Pi_res[r_size][res_size:1];


endmodule