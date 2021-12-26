`timescale 1ns / 1ps 
module lab3_2(
			input[3:0] number,
			input CLK, 
			input selection,
			input mode,
			input clear,
			output reg [7:0] digit1,
			output reg [7:0] digit0,
			output reg [7:0] count1,
			output reg [7:0] count0,
			output reg warning
    );
   //Modify the lines below to implement your design .


		reg [3:0] digit1_prime;
		reg [3:0] digit0_prime;
		reg [3:0] count1_prime;
		reg [3:0] count0_prime;
		reg [3:0] digit1_nonprime;
		reg [3:0] digit0_nonprime;
		reg [3:0] count1_nonprime;
		reg [3:0] count0_nonprime;
	initial begin

		count1 = 0;
		count0 = 0;
		digit1 = 0;
		digit0 = 0;
		digit1_prime=0;
		digit0_prime=0;
		count1_prime=0;
		count0_prime=0;
		
		digit1_nonprime=0;
		digit0_nonprime=0;
		count1_nonprime=0;
		count0_nonprime=0;
		
		warning=0;
		
	end

	always@(posedge CLK) begin
	if (clear==1) begin 
		count1 = 0;
		count0 = 0;
		digit1 = 0;
		digit0 = 0;
		digit1_prime=0;
		digit0_prime=0;
		count1_prime=0;
		count0_prime=0;
		
		digit1_nonprime=0;
		digit0_nonprime=0;
		count1_nonprime=0;
		count0_nonprime=0;
		
		warning=0;
	end
	else begin

		if (selection == 0) begin
			if(number==4'd2 || number==4'd3 || number== 4'd5 || number==4'd7  || number==4'd11 || number==4'd13) begin
				warning =1'b0;
				if(mode==1)begin
					if(number==4'b0010)begin
						digit0_prime=number;
						digit1_prime=4'b0011;
						
						if(count1_prime==8'd9) begin
						count1_prime = 4'd0;
						end
						else begin
						count1_prime=count1_prime + 1;
						end
						
						
						count0=count0_prime;
						count1=count1_prime;
						digit0=digit0_prime;
						digit1=digit1_prime;
					end
					
					if(number==4'b0011)begin
						digit0_prime=number;
						digit1_prime=4'b0101;
						
						if(count1_prime==4'd9) begin
						count1_prime = 4'd0;
						end
						else begin
						count1_prime=count1_prime + 1;
						end
						
						count0=count0_prime;
						count1=count1_prime;
						digit0=digit0_prime;
						digit1=digit1_prime;
						
					end
					
					else if(number==4'b0101)begin
						digit0_prime=number;
						digit1_prime=4'b0111;
						
						if(count1_prime==8'd9) begin
						count1_prime = 4'd0;
						end
						else begin
						count1_prime=count1_prime + 1;
						end
						
						count0=count0_prime;
						count1=count1_prime;
						digit0=digit0_prime;
						digit1=digit1_prime;
					end
					
					else if(number==4'b0111)begin
						digit0_prime=number;
						digit1_prime=4'b1011;
						
						if(count1_prime==4'd9) begin
						count1_prime = 4'd0;
						end
						else begin
						count1_prime=count1_prime + 1;
						end
						
						count0=count0_prime;
						count1=count1_prime;
						digit0=digit0_prime;
						digit1=digit1_prime;
					end
					
					else if(number==4'b1011)begin
						digit0_prime=number;
						digit1_prime=4'b1101;
						
						if(count1_prime==4'd9) begin
						count1_prime = 4'd0;
						end
						else begin
						count1_prime=count1_prime + 1;
						end
						
						count0=count0_prime;
						count1=count1_prime;
						digit0=digit0_prime;
						digit1=digit1_prime;
					end
					
					else if(number==4'b1101)begin
						digit0_prime=number;
						digit1_prime=4'b0010;
						
						if(count1_prime==4'd9) begin
						count1_prime = 4'd0;
						end
						else begin
						count1_prime=count1_prime + 1;
						end
						
						count0=count0_prime;
						count1=count1_prime;
						digit0=digit0_prime;
						digit1=digit1_prime;
					end
					
				end
				else if (mode==0) begin
					
					if(number==4'b0010)begin
						digit0_prime=number;
						digit1_prime=4'b1101;
						
						if(count0_prime==4'd9) begin
						count0_prime = 4'd0;
						end
						else begin
						count0_prime=count0_prime + 1;
						end
						
						count0=count0_prime;
						count1=count1_prime;
						digit0=digit0_prime;
						digit1=digit1_prime;
						
					end
					
					if(number==4'b0011)begin
						digit0_prime=number;
						digit1_prime=4'b0010;
						
						if(count0_prime==4'd9) begin
						count0_prime = 4'd0;
						end
						else begin
						count0_prime=count0_prime + 1;
						end
						
						count0=count0_prime;
						count1=count1_prime;
						digit0=digit0_prime;
						digit1=digit1_prime;
						
					end
					
					else if(number==4'b0101)begin
						digit0_prime=number;
						digit1_prime=4'b0011;
						
						if(count0_prime==4'd9) begin
						count0_prime = 4'd0;
						end
						else begin
						count0_prime=count0_prime + 1;
						end
						
						count0=count0_prime;
						count1=count1_prime;
						digit0=digit0_prime;
						digit1=digit1_prime;
					end
					
					else if(number==4'b0111)begin
						digit0_prime=number;
						digit1=4'b0101;
						
						if(count0_prime==4'd9) begin
						count0_prime = 4'd0;
						end
						else begin
						count0_prime=count0_prime + 1;
						end
						
						count0=count0_prime;
						count1=count1_prime;
						digit0=digit0_prime;
						digit1=digit1_prime;
					end
					
					else if(number==4'b1011)begin
						digit0_prime=number;
						digit1_prime=4'b0111;
						
						if(count0_prime==4'd9) begin
						count0_prime = 4'd0;
						end
						else begin
						count0_prime=count0_prime + 1;
						end
						
						count0=count0_prime;
						count1=count1_prime;
						digit0=digit0_prime;
						digit1=digit1_prime;
					end
					
					else if(number==4'b1101)begin
						digit0_prime=number;
						digit1_prime=4'b1011;
						
						if(count0_prime==4'd9) begin
						count0_prime = 4'd0;
						end
						else begin
						count0_prime=count0_prime + 1;
						end
						
						count0=count0_prime;
						count1=count1_prime;
						digit0=digit0_prime;
						digit1=digit1_prime;
					end
					
				end
				
			end
			else begin
			warning =1'b1;
			end
		end
		else if (selection==1) 
			begin
				if(number!=4'd2 && number!=4'd3 && number!= 4'd5 && number!=4'd7  && number!=4'd11 && number!=4'd13) begin
				warning = 1'b0;
					if(mode==1) begin
						digit0_nonprime=number;
						
						if(number==4'd0) begin
						digit1_nonprime = 4'd0;
						end
						else if(number==4'd1) begin
						digit1_nonprime = 4'd2;
						end
						
						else if(number==4'd4) begin
						digit1_nonprime = 4'd8;
						end
						else if(number==4'd6) begin
						digit1_nonprime = 4'd12;
						end
						else if(number==4'd8) begin
						digit1_nonprime = 4'd0;
						end
						else if(number==4'd9) begin
						digit1_nonprime = 4'd2;
						end
						else if(number==4'd10) begin
						digit1_nonprime = 4'd4;
						end
						else if(number==4'd12) begin
						digit1_nonprime = 4'd8;
						end
						else if(number==4'd14) begin
						digit1_nonprime = 4'd12;
						end
						else if(number==4'd15) begin
						digit1_nonprime = 4'd14;
						end
						
						
						
						
						
						if(count1_nonprime==4'd9) begin
						count1_nonprime = 4'd0;
						end
						else begin
						count1_nonprime=count1_nonprime + 1;
						end
						
						
						count0 = count0_nonprime;
						count1 = count1_nonprime;
						digit1 = digit1_nonprime;
						digit0 = digit0_nonprime;
						
					end
					
					else if(mode==0) begin
						digit0_nonprime= number;
						
						if(number==4'd0)begin
						digit1_nonprime = 4'd0;
						end
						
						else if(number==4'd1) begin
						digit1_nonprime = 4'd0;
						end
						else if(number==4'd4) begin
						digit1_nonprime = 4'd2;
						end
						else if(number==4'd6) begin
						digit1_nonprime = 4'd3;
						end
						else if(number==4'd8) begin
						digit1_nonprime = 4'd4;
						end
						else if(number==4'd9) begin
						digit1_nonprime = 4'd4;
						end
						else if(number==4'd10) begin
						digit1_nonprime = 4'd5;
						end
						else if(number==4'd12) begin
						digit1_nonprime = 4'd6;
						end
						else if(number==4'd14) begin
						digit1_nonprime = 4'd7;
						end
						else if(number==4'd15) begin
						digit1_nonprime = 4'd7;
						end
						
						
						if(count0_nonprime==4'd9) begin
						count0_nonprime = 4'd0;
						end
						else begin
						count0_nonprime=count0_nonprime + 1;
						end
						
						count1 = count1_nonprime;
						count0 = count0_nonprime;
						digit1 = digit1_nonprime;
						digit0 = digit0_nonprime;
					end
				end
				else begin
				warning = 1'b1;
				end
			end
		
		end
	end	



endmodule

