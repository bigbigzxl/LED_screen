`timescale 1ns/1ns
module tb_fir();

reg clk;
reg rst;
reg signed [31:0] data_mem[999:0];
reg signed [31:0] data_buffer;
reg  [9:0] i;
wire signed [31:0] data_out;
// wire signed [57:0] debug;

initial begin
    $readmemh("E:\\FPGA\\proj\\fir\\data_in.txt", data_mem);
    for(i=0;i<=2;i=i+1)   //把八个存储单元的数字都读取出来，若存的数不到八个单元输出x态，程序结果中会看到
		$display("%x",data_mem[i]);

    #0 clk <= 1'b0;
    #0 rst <= 1'b1;
    #5 rst <= 1'b0;
end

//  clock generate
always #10 clk = ~clk; 

always @(posedge clk or negedge rst) begin
    if (rst == 1'b1)
        begin
            i <= 10'd0;
        end
    else if (i == 10'd999)
        i <= 10'b0;
    else
        begin
            i <= i + 10'd1;
        end
end

always @(posedge clk or negedge rst) begin
    if (rst == 1'b1)
        begin
            data_buffer <= 32'b0;
        end
    else
        begin
            data_buffer <= data_mem[i];
        end
end

fir40_min fir_inst(
    .clk(clk),
    .rst(rst),
    .data_in(data_buffer),
    .data_out(data_out)
);

endmodule