`timescale 1ns / 1ps

module df(
    input d,
    input f,
    input clk,
    output reg q
    );
    
    initial begin
        q = 0;
    end
	
    // write your code here
	
	 always@(posedge clk)
    begin
    case({d, f})
        2'b00: q <= 1'b1;
        2'b01: q <= q;
        2'b10: q <= 1'b0;
        2'b11: q <= ~q;
    endcase
    end


endmodule


module icplusplus(
    input d0, 
    input f0, 
    input d1, 
    input f1, 
    input clk, 
    output q0, 
    output q1, 
    output y
    );
    // write your code here

        df df0(d0, f0, clk, q0);
        df df1(d1, f1, clk, q1);
        assign y = ~(q0 ^ q1);

endmodule