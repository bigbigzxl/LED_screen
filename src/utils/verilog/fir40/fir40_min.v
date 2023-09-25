module fir40_min(
    input  wire                  clk,
    input  wire                  rst,
    input  wire signed [31:0]    data_in,
    output wire signed [31:0]    data_out
);

reg [1:0] cnt;
// define parameter and internal signals
reg signed [18:0] tap_40[19:0];
reg signed [31:0] h_40[39:0];
reg signed [57:0] mul[4:0];

reg signed [57:0] acc; // 40 -> 6bit + 18bit + 32 bit
reg signed [57:0] mac; // 40 -> 6bit + 18bit + 32 bit


assign data_out = mac >>> 18;
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
end


always @(posedge clk or posedge rst)
begin
    if (rst == 1'b1)
        cnt <= 2'd0;
    else
        cnt <= cnt + 1'd1;
end

always @(posedge clk or posedge rst)
begin
    if (rst == 1'b1)
        h_40[0] <= 32'd0;
    else if(cnt == 1'b0)
        h_40[0] <= data_in;
    else
        h_40[0] <= h_40[0];
end

generate
    genvar i;
    for (i = 0; i < 40 - 1; i = i + 1)
        begin:h_gen
            always@(posedge clk or posedge rst)
                if(rst == 1'b1)
                    h_40[i+1] <= 32'b0;
                else if(cnt == 1'b0)
                    h_40[i+1] <= h_40[i];
                else
                    h_40[i+1] <= h_40[i+1];
        end
endgenerate


always @(*) begin: Mult
    integer  t;
    case (cnt)
    0: begin 
        for (t = 0; t < 5; t = t + 1)
        begin
            mul[t] <= tap_40[t] * (h_40[t] +  h_40[39-t]);
        end
    end
    1: begin 
        for (t = 5; t < 10; t = t + 1)
        begin
            mul[t-5] <= tap_40[t] * (h_40[t] +  h_40[39-t]);
        end
    end
    2: begin 
        for (t = 10; t < 15; t = t + 1)
        begin
            mul[t-10] <= tap_40[t] * (h_40[t] +  h_40[39-t]);
        end
    end
    3: begin 
        for (t = 15; t < 20; t = t + 1)
        begin
            mul[t-15] <= tap_40[t] * (h_40[t] +  h_40[39-t]);
        end
    end
    endcase
end

always @(*) begin: ACC
    if (cnt == 1'b0)
        acc = mul[0] + mul[1] + mul[2] + mul[3] + mul[4];
    else
        acc <= mul[0] + mul[1] + mul[2] + mul[3] + mul[4] + acc;
end

always @(*) begin: OUT
    if (rst == 1'b1)
        mac <= 58'd0;
    else if(cnt == 2'd3)
        mac <= acc;
    else
        mac <= mac;
        
end

endmodule