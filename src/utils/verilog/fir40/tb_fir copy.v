`timescale 1ns/1ns
module tb_fir();

reg clk;
reg rst_n;
reg signed [31:0] data_mem[999:0];
reg signed [31:0] data_buffer;
reg  [9:0] i;
wire signed [31:0] data_out;
// wire signed [57:0] debug;
wire data_out_en;

initial begin
    $readmemh("E:\\FPGA\\proj\\fir\\data_in.txt", data_mem);
    for(i=0;i<=2;i=i+1)   //把八个存储单元的数字都读取出来，若存的数不到八个单元输出x态，程序结果中会看到
		$display("%x",data_mem[i]);

    #0 clk <= 1'b0;
    #0 rst_n <= 1'b0;
    #5 rst_n <= 1'b1;
end

//  clock generate
always #10 clk = ~clk; 

always @(posedge clk or negedge rst_n) begin
    if (rst_n == 1'b0)
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

always @(posedge clk or negedge rst_n) begin
    if (rst_n == 1'b0)
        begin
            data_buffer <= 32'b0;
        end
    else
        begin
            data_buffer <= data_mem[i];
        end
end

fir40 fir_inst(
    .m_clk(clk),
    .rst_n(rst_n),
    .data_in(data_buffer),
    .data_in_en(clk),
    .data_out_en(data_out_en),
    .data_out(data_out),
    // .debug(debug)
);

endmodule