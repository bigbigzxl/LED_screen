module fir40(
    input  wire                  m_clk,
    input  wire                  rst_n,
    input  wire signed [31:0]    data_in,
    input  wire                  data_in_en,
    output reg                   data_out_en,
    output wire signed [31:0]    data_out
);

// define parameter and internal signals
reg signed [18:0] tap_40[39:0];
reg signed [31:0] h_40[39:0];
reg signed [57:0] mul_40[19:0];

reg signed [57:0] dout_tap0[3:0]; 
reg signed [57:0] dout_tap1[1:0]; 
reg signed [57:0] h_out; // 40 -> 6bit + 18bit + 32 bit
reg data_in_en_r1;
reg data_in_en_r2;
reg data_in_en_r3;
reg data_in_en_r4;

assign debug = mul_40[1];  // debug
assign data_out = h_out >>> 18;

initial 
begin
    tap_40[0]  = 0;     
    tap_40[1]  = 2;     
    tap_40[2]  = 7;     
    tap_40[3]  = 8;   
    tap_40[4]  = -19;   
    tap_40[5]  = -98;  
    tap_40[6]  = -194;  
    tap_40[7]  = -150;   
    tap_40[8]  = 263;
    tap_40[9]  = 1060;  
    tap_40[10] = 1700;  
    tap_40[11] = 1091; 
    tap_40[12] = -1642; 
    tap_40[13] = -5833; 
    tap_40[14] = -8463; 
    tap_40[15] = -5072;  
    tap_40[16] = 7453; 
    tap_40[17] = 27824;
    tap_40[18] = 49531; 
    tap_40[19] = 63603; 
    tap_40[20] = 63603; 
    tap_40[21] = 49531; 
    tap_40[22] = 27824;  
    tap_40[23] = 7453; 
    tap_40[24] = -5072; 
    tap_40[25] = -8463; 
    tap_40[26] = -5833;
    tap_40[27] = -1642;  
    tap_40[28] = 1091;  
    tap_40[29] = 1700;  
    tap_40[30] = 1060;   
    tap_40[31] = 263;  
    tap_40[32] = -150;  
    tap_40[33] = -194;   
    tap_40[34] = -98;   
    tap_40[35] = -19;
    tap_40[36] = 8;     
    tap_40[37] = 7;     
    tap_40[38] = 2;     
    tap_40[39] = 0;    
end

always @(posedge m_clk or negedge rst_n)
begin
    if (rst_n == 1'b0)
        h_40[0] <= 32'd0;
    else if(data_in_en == 1'b1)
        h_40[0] <= data_in;
    else
        h_40[0] <= h_40[0];
end

generate
    genvar i;
    for (i = 0; i < 40 - 1; i = i + 1)
        begin:h_gen
            always@(posedge m_clk or negedge rst_n)
                if(rst_n == 1'b0)
                    h_40[i+1] <= 32'b0;
                else if(data_in_en == 1'b1)
                    h_40[i+1] <= h_40[i];
                else
                    h_40[i+1] <= h_40[i+1];
        end
endgenerate

generate
    genvar j;
    for (j = 0; j < 20; j = j + 1)
        begin : mul_gen
            always@(posedge m_clk or negedge rst_n)
                if(rst_n == 1'b0)
                begin
                    mul_40[j] <= 57'b0;
                end
                else if(data_in_en == 1'b1)
                    mul_40[j] <= tap_40[j] * (h_40[j] +  h_40[39-j]);
                else
                begin
                    mul_40[j] <= mul_40[j];
                end
        end
endgenerate

always@(posedge m_clk or negedge rst_n)
begin
    if(rst_n == 1'b0)
    begin
        dout_tap0[0] <= 57'b0;
        dout_tap0[1] <= 57'b0;
        dout_tap0[2] <= 57'b0;
        dout_tap0[3] <= 57'b0;
    end
    else if(data_in_en == 1'b1)
    begin
        dout_tap0[0] <= mul_40[0]  + mul_40[1] + mul_40[2] + mul_40[3] + mul_40[4];
        dout_tap0[1] <= mul_40[5] + mul_40[6] + mul_40[7] + mul_40[8] + mul_40[9]; 
        dout_tap0[2] <= mul_40[10] + mul_40[11] + mul_40[12] + mul_40[13] + mul_40[14];
        dout_tap0[3] <= mul_40[15] + mul_40[16] + mul_40[17] + mul_40[18] + mul_40[19]; 
        end
    else
    begin
        dout_tap0[0] <= dout_tap0[0];
        dout_tap0[1] <= dout_tap0[1];
        dout_tap0[2] <= dout_tap0[2];
        dout_tap0[3] <= dout_tap0[3];
        end
end

always@(posedge m_clk or negedge rst_n)
begin
    if(rst_n == 1'b0) begin
        dout_tap1[0] <= 57'b0;
        dout_tap1[1] <= 57'b0;
        end
    else if(data_in_en == 1'b1) begin
        dout_tap1[0] <= dout_tap0[0] + dout_tap0[1];
        dout_tap1[1] <= dout_tap0[2] + dout_tap0[3];
        end
    else begin
        dout_tap1[0] <=  dout_tap1[0];
        dout_tap1[1] <=  dout_tap1[1];
        end
end

always@(posedge m_clk or negedge rst_n)
begin
    if(rst_n == 1'b0)
        h_out <= 57'b0;
    else if(data_in_en == 1'b1)
        h_out <= dout_tap1[0] +dout_tap1[1];
    else
        h_out <= h_out;
end

always@(posedge m_clk or negedge rst_n)
if(rst_n == 1'b0)
begin
    data_in_en_r1 <= 1'b0;
    data_in_en_r2 <= 1'b0;
    data_in_en_r3 <= 1'b0;
    data_in_en_r4 <= 1'b0;
    data_out_en   <= 1'b0;
end
else
begin
    data_in_en_r1 <= data_in_en;
    data_in_en_r2 <= data_in_en_r1;
    data_in_en_r3 <= data_in_en_r2;
    data_in_en_r4 <= data_in_en_r3;
    data_out_en <= data_in_en_r4;
end
endmodule