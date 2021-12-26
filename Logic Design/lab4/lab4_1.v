`timescale 1ns / 1ps
module MaskROM (input [3:0] RomAddress, output reg[7:0] RomDataOut);
/*  Please write your code below  */
	always@(*)begin
		case(RomAddress)
			4'b0000: RomDataOut = 8'b00000000;
			4'b0001: RomDataOut = 8'b00001111;
			4'b0010: RomDataOut = 8'b00011110;
			4'b0011: RomDataOut = 8'b00110000;
			4'b0100: RomDataOut = 8'b01010000;
			4'b0101: RomDataOut = 8'b01100110;
			4'b0110: RomDataOut = 8'b01101010;
			4'b0111: RomDataOut = 8'b01111110;
			4'b1000: RomDataOut = 8'b10000001;
			4'b1001: RomDataOut = 8'b10100000;
			4'b1010: RomDataOut = 8'b10100110;
			4'b1011: RomDataOut = 8'b10111101;
			4'b1100: RomDataOut = 8'b11000000;
			4'b1101: RomDataOut = 8'b11010000;
			4'b1110: RomDataOut = 8'b11010011;
			4'b1111: RomDataOut = 8'b11100110;
			
		endcase
	end

/*  Please write your code above  */
endmodule


/*mode 0:read, 1:write*/																							
module RgbRAM (input Mode,input [3:0] RamAddress, input [23:0] RamDataIn,
					input [7:0] Mask,input [2:0] Op, input CLK, output reg [23:0] RamDataOut);
/*  Please write your code below  */
	reg [23:0] ram[0:15];
	reg [7:0] red;
	reg [7:0] green;
	reg [7:0] blue;
	integer temp;
	
	initial begin 
		ram[0] = 24'b000000000000000000000000;
		ram[1] = 24'b000000000000000000000000;
		ram[2] = 24'b000000000000000000000000;
		ram[3] = 24'b000000000000000000000000;
		ram[4] = 24'b000000000000000000000000;
		ram[5] = 24'b000000000000000000000000;
		ram[6] = 24'b000000000000000000000000;
		ram[7] = 24'b000000000000000000000000;
		ram[8] = 24'b000000000000000000000000;
		ram[9] = 24'b000000000000000000000000;
		ram[10] = 24'b000000000000000000000000;
		ram[11] = 24'b000000000000000000000000;
		ram[12] = 24'b000000000000000000000000;
		ram[13] = 24'b000000000000000000000000;
		ram[14] = 24'b000000000000000000000000;
		ram[15] = 24'b000000000000000000000000;
		
	   red = 8'b0;
		green = 8'b0;
		blue = 8'b0;
	end
	
	
	
		always@(Mode,RamAddress) begin
			if (Mode==0) begin
		//read
			RamDataOut = ram[RamAddress];
		end
	end
	always@(posedge CLK) begin
		if (Mode==1) begin
			
			if(Op==3'b000)begin
				{red,green,blue}=RamDataIn;
				red=red & Mask ;
				green=green & Mask;
				blue = blue & Mask;
				ram[RamAddress]={red,green,blue};
				
			end
			
			else if(Op==3'b001)begin
				{red,green,blue}=RamDataIn;
				red=red | Mask ;
				green=green | Mask;
				blue = blue | Mask;
				ram[RamAddress]={red,green,blue};
			end
			
			else if(Op==3'b010)begin
				{red,green,blue}=RamDataIn;
				red=red ^ Mask ;
				green=green ^ Mask;
				blue = blue ^ Mask;
				ram[RamAddress]={red,green,blue};
				
			end
			
			else if(Op==3'b011)begin
				{red,green,blue}=RamDataIn;
				temp=red+Mask;
				if(temp >255)begin
				red=255;
				end
				else begin
				red=red + Mask ;
				end
				
				temp=green+Mask;
				if(temp >255)begin
				green=255;
				end
				else begin
				green=green + Mask;
				end
				
				temp=blue+Mask;
				if(temp >255)begin
				blue=255;
				end
				else begin
				blue = blue + Mask;
				end
				
				
				
				ram[RamAddress]={red,green,blue};
			end
			
			else if(Op==3'b100)begin
				{red,green,blue}=RamDataIn;
				if(red < Mask)begin
					red=0;
				end
				else begin
				red=red - Mask ;
				end
				
				if(green < Mask)begin
					green=0;
				end
				else begin
				green=green - Mask ;
				end
				
				if(blue < Mask)begin
					blue=0;
				end
				else begin
				blue=blue - Mask ;
				end
				
				
				ram[RamAddress]={red,green,blue};
			end
		
			else if(Op==3'b101)begin
				{red,green,blue}=RamDataIn;
				if(red + 8'b00000001 >255)
					red=255;
				else begin
					red=red+ 8'b00000001;
				end
				
				if(green + 8'b00000001 >255)
					green=255;
				else begin
					green=green+ 8'b00000001;
				end
				
				if(blue + 8'b00000001 >255)
					blue=255;
				else begin
					blue=blue+ 8'b00000001;
				end
				
				ram[RamAddress]={red,green,blue};
			end
			
			else if(Op==3'b110)begin
				{red,green,blue}=RamDataIn;
				if(red== 0)
				red=0;
				else begin
				red=red-8'b00000001;
				end
				
				if(green == 0)
				green=0;
				else begin
				green=green-8'b00000001;
				end
				
				if(blue == 0)
				blue=0;
				else begin
				blue=blue-8'b00000001;
				end
				
			ram[RamAddress]={red,green,blue};
			end
			
			else if(Op==3'b111)begin
				{red,green,blue}=RamDataIn;
				red={red[6:0],red[7]};
				green={green[6:0],green[7]};
				blue={blue[6:0],blue[7]};
				ram[RamAddress]={red,green,blue};
				
			end
			
			
		
		end
	
	
end

/*  Please write your code above  */
endmodule


module RgbMaskModule(input Mode, input [3:0] Address, input [23:0] RGBin,input [2:0] Op,  input CLK, output wire [23:0] RGBout);
	
	/*  DO NOT edit this module  */
	
	wire [7:0]  romResult;

	MaskROM RO(Address, romResult);
	RgbRAM RA(Mode, Address, RGBin,romResult, Op, CLK, RGBout);
endmodule
