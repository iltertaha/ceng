`timescale 1ns / 1ps

module lab4_2(
  input [1:0] mode,
  input [2:0] n_gasoline_pumps,      // for setup mode, max 6
  input [2:0] n_diesel_pumps,        // for setup mode, max 6
  input [3:0] fuel_amount,           // for add car mode, max 8
  input fuel_type,                   // for add car mode, gas(0) or diesel(1)
  input CLK, 

  output reg [0:5] pump_status,                // pump is working or not, max 6 pumps
  output reg is_gasoline_queue_not_full,       // gasoline queue full warning
  output reg is_diesel_queue_not_full,         // diesel queue full warning

  output reg [3:0] n_cars_in_gasoline_queue,   // to represent max 9 on 1 7-seg display
  output reg [3:0] n_cars_in_diesel_queue,     // to represent max 9 on 1 7-seg display

  output reg [7:0] total_gasoline_needed,      // to represent max 99 on 2 7-seg displays
  output reg [7:0] total_diesel_needed,        // to represent max 99 on 2 7-seg displays

  // reset these after one clock
  output reg invalid_gasoline_car,             // to lightup dot at an2
  output reg invalid_diesel_car,               // to lightup dot at an0
  output reg invalid_setup_params              // to lightup all dots
);

// write your code here

   reg [2:0] n2_gasoline_pumps;      // for setup mode, max 6
   reg [2:0] n2_diesel_pumps;      // for setup mode, max 6
	
	reg [3:0] gas_queue [7:0];
	reg [3:0] diesel_queue [7:0];
	
	reg setup_flag; //check whether it setted before sim
	integer i; //for iteration
	(* keep = "true" *)  reg added;
	(* keep = "true" *)  reg [0:5] azaltma;
	reg [3:0] front_gasoline;  //for queue
	reg [3:0] front_diesel;//for queue
	reg [3:0] back_gasoline;//for queue
	reg [3:0] back_diesel;//for queue
	
	
	//integer diesel_pump;//unused
	//integer gas_pump;//unused
	
	reg [3:0] pump_current [5:0]; //her pompann doldurmas gereken kalan yakit


	initial begin 
		setup_flag=1'b0;
		added=1'b0;
		for(i=0;i<8;i=i+1)
		begin
			gas_queue[i]=0;
			diesel_queue[i]=0;
			
		end
		
		front_gasoline=0; //queue empty
		back_gasoline=7;
		is_gasoline_queue_not_full = 0;
		
		front_diesel=0;//queue empty
		back_diesel=7;
		is_diesel_queue_not_full = 0;
			
		pump_status = 6'b000000;
		azaltma=6'b000000;
		
		n_cars_in_gasoline_queue = 4'b1111;
		n_cars_in_diesel_queue = 4'b1111;
		
		total_gasoline_needed = 8'b11111111;
		total_diesel_needed = 8'b11111111;
		
		invalid_gasoline_car =0;
		invalid_diesel_car = 0;
		invalid_setup_params = 0;
		
	end
		always@(posedge CLK) begin
		
		if(mode==2'b10 || mode==2'b11)begin
				
				
				if(n_gasoline_pumps+n_diesel_pumps > 6 || n_gasoline_pumps+n_diesel_pumps==0 )begin
					invalid_setup_params = 1; //error case
					setup_flag=1;
				end
				
				else begin
				n2_diesel_pumps=n_diesel_pumps;
				n2_gasoline_pumps=n_gasoline_pumps;
				setup_flag=1; //hatal da olsa girmis sayilir mi
				is_diesel_queue_not_full = 1;
				is_gasoline_queue_not_full =1 ;
				azaltma=6'b000000;
				
					for(i=0;i<8;i=i+1)begin
					
						gas_queue[i]=0;
						diesel_queue[i]=0;
						
					end
					front_gasoline=0;
					back_gasoline=7;
					front_diesel=0;
					back_diesel=7;
						
					pump_status = 6'b000000;
					
					is_gasoline_queue_not_full = 0;
					is_diesel_queue_not_full = 0;
					
					n_cars_in_gasoline_queue = 4'b0000;
					n_cars_in_diesel_queue = 4'b0000;
					
					total_gasoline_needed = 8'b00000000;
					total_diesel_needed = 8'b00000000;
					
					invalid_gasoline_car =0;
					invalid_diesel_car = 0;
					invalid_setup_params = 0;
					
					if(n2_diesel_pumps==0)begin //only gas
						n_cars_in_diesel_queue = 4'b1111;
						total_diesel_needed= 8'b11111111;
						is_gasoline_queue_not_full = 1;
						is_diesel_queue_not_full = 0;
						
						case(n2_gasoline_pumps)
							1: pump_status=6'b100000;
							2: pump_status=6'b110000;
							3: pump_status=6'b111000;
							4: pump_status=6'b111100;
							5: pump_status=6'b111110;
							6: pump_status=6'b111111;
						endcase
					end
					
					else if(n2_gasoline_pumps==0)begin //only diesel
						n_cars_in_gasoline_queue = 4'b1111;
						total_gasoline_needed= 8'b11111111;
						is_gasoline_queue_not_full = 0;
						is_diesel_queue_not_full = 1;
						case(n2_diesel_pumps)
							1: pump_status=6'b100000;
							2: pump_status=6'b110000;
							3: pump_status=6'b111000;
							4: pump_status=6'b111100;
							5: pump_status=6'b111110;
							6: pump_status=6'b111111;
							
						endcase
					end
					
					else begin
						n_cars_in_gasoline_queue = 4'b0000;
						n_cars_in_diesel_queue = 4'b0000;
						is_gasoline_queue_not_full = 1;
						is_diesel_queue_not_full = 1;
					
						total_gasoline_needed = 8'b00000000;
						total_diesel_needed = 8'b00000000;
						
						case(n2_gasoline_pumps+n2_diesel_pumps)
							2:pump_status=6'b110000;
							3:pump_status=6'b111000;
							4:pump_status=6'b111100;
							5:pump_status=6'b111110;
							6:pump_status=6'b111111;
						endcase
					end
				end
			end
			
			
		else if(mode==2'b00 && setup_flag==1)begin
			invalid_gasoline_car =0;
			invalid_diesel_car = 0;
			invalid_setup_params = 0;
			
			/*only gas*/
			
			if(n2_diesel_pumps==0)begin
				/*move cars to available pumps*/
				azaltma=6'b000000;
				for(i=0;i<6;i=i+1)begin
					if(i<n2_gasoline_pumps)begin
						if(pump_status[i]==1 && !(is_gasoline_queue_not_full==1 &&(back_gasoline+1)%8 == front_gasoline))begin //if queue is not empty
							/*total_gasoline_needed= total_gasoline_needed + gas_queue[front_gasoline];*/ //add new car amount 
							pump_current[i]=gas_queue[front_gasoline]; //update pump
							n_cars_in_gasoline_queue= n_cars_in_gasoline_queue -1;
							
							
							/*delete front from queue*/
							front_gasoline = (front_gasoline + 1) % 8; //dequeue
							is_gasoline_queue_not_full=1; //queue not full
							pump_status[i]=0; //busy
							
							azaltma[i]=1;
							
						end
					end
					
				end
			
			
			
			/*1 time simulation*/
			
				for(i=0;i<6;i=i+1)begin
					if(i<n2_gasoline_pumps)begin //iterate over used gasoline pumps
						if(pump_status[i]==0 && azaltma[i]==0)begin //if pump is in use
							total_gasoline_needed=total_gasoline_needed - 1; //decrement total
							pump_current[i]=pump_current[i] - 1;  //decrement that pump
						end
						
						if(pump_current[i]==0)begin //if complete
							pump_status[i]=1; //make pump available again
						end
					end
				end
				azaltma=6'b000000;
			end
			/*------------------------------------------------------------*/
			
			else if(n2_gasoline_pumps==0)begin
				azaltma=6'b000000;
				
				/* ---new car goes to pump--*/
				for(i=0;i<6;i=i+1)begin
					if(i<n2_diesel_pumps)begin
						if(pump_status[i]==1 && !(is_diesel_queue_not_full==1 && (back_diesel+1)%8 == front_diesel))begin
							/*total_diesel_needed= total_diesel_needed + diesel_queue[front_diesel]; *///update
							pump_current[i]=diesel_queue[front_diesel];
							n_cars_in_diesel_queue= n_cars_in_diesel_queue -1;
							/*delete front from queue*/
							front_diesel = (front_diesel + 1) % 8; //diesel
							is_diesel_queue_not_full=1;
							pump_status[i]=0;
							azaltma[i]=1;
							
						end
					end
				end
			
			

				for(i=0;i<6;i=i+1)begin //iterate over used diesel pumps
					if(i<n2_diesel_pumps)begin
						if(pump_status[i]==0  && azaltma[i]==0)begin //if pump is in use
							total_diesel_needed=total_diesel_needed - 1; //decrement total
							pump_current[i]=pump_current[i] - 1;  //decrement that pump
							if(pump_current[i]==0)begin //if complete
								pump_status[i]=1; //make pump available again
							end
						end
					end
				end
				
				azaltma=6'b000000;
			end
			/*----------------------*/
			else begin
			
				azaltma=6'b000000;
			
				for(i=0;i<6;i=i+1)begin //first add gas if available
					if(i<n2_gasoline_pumps)begin
						if(pump_status[i]==1 && !(is_gasoline_queue_not_full==1 && (back_gasoline+1)%8 == front_gasoline))begin // if there is a car in queue
							/*total_gasoline_needed= total_gasoline_needed + gas_queue[front_gasoline];*/ //update
							pump_current[i]=gas_queue[front_gasoline];
							n_cars_in_gasoline_queue= n_cars_in_gasoline_queue - 1;
 							
							front_gasoline=(front_gasoline + 1)%8 ; //dequeue
							is_gasoline_queue_not_full = 1;
							pump_status[i]=0; //busy
							azaltma[i]=1;
						end
					end
				end
				
				for(i=0;i<6;i=i+1)begin //then add diesel if available
					if(i<n2_diesel_pumps)begin
						if(pump_status[i+n2_gasoline_pumps]==1 && !(is_diesel_queue_not_full==1 && (back_diesel+1)%8 == front_diesel) )begin
							/*total_diesel_needed= total_diesel_needed + diesel_queue[front_diesel];*/ //update
							pump_current[i+n2_gasoline_pumps]=diesel_queue[front_diesel];
							n_cars_in_diesel_queue= n_cars_in_diesel_queue - 1;
							
							front_diesel=(front_diesel + 1)% 8; //dequeue
							is_diesel_queue_not_full=1;
							pump_status[i+n2_gasoline_pumps]=0; //busy
							azaltma[i+n2_gasoline_pumps]=1;
						end
					end
				end
				
				
				/*simulate for 1 time*/
				for(i=0;i<6;i=i+1)begin //iterate over used gasoline pumps
					if(i<n2_gasoline_pumps)begin
						if(pump_status[i]==0 && azaltma[i]==0)begin //if pump is in use
							total_gasoline_needed=total_gasoline_needed - 1; //decrement total
							pump_current[i]=pump_current[i] - 1;  //decrement that pump
							
							if(pump_current[i]==0)begin //if complete
								pump_status[i]=1; //make pump available again
							end
						end
					end
				end
				
				for(i=0;i<6;i=i+1)begin //iterate over used diesel pumps
					if(i<n2_diesel_pumps)begin
						if(pump_status[i+n2_gasoline_pumps]==0 && azaltma[i+n2_gasoline_pumps]==0)begin //if pump is in use
							total_diesel_needed=total_diesel_needed - 1; //decrement total
							pump_current[i+n2_gasoline_pumps]=pump_current[i+n2_gasoline_pumps] - 1;  //decrement that pump
							
							if(pump_current[i+n2_gasoline_pumps]==0)begin //if complete
								pump_status[i+n2_gasoline_pumps]=1; //make pump available again
							end
						end
					end
				end
				
				azaltma=6'b000000;
				
			end//end of else block	
			
			

		end//mode1 sonu
		
		else if(mode==2'b01)begin //addcar 
			invalid_gasoline_car = 0;
			invalid_diesel_car = 0;
			invalid_setup_params=0;
			
			
			if(n2_diesel_pumps==0)begin //only gas
				
				/*araclar boslara yonlendir*/
				
				
				/*move cars to available pumps*/
					azaltma=6'b000000;
					for(i=0;i<6;i=i+1)begin
						if(i<n2_gasoline_pumps)begin
							if(pump_status[i]==1 && !(is_gasoline_queue_not_full==1 &&(back_gasoline+1)%8 == front_gasoline))begin //if queue is not empty
								/*total_gasoline_needed= total_gasoline_needed + gas_queue[front_gasoline];*/ //add new car amount 
								pump_current[i]=gas_queue[front_gasoline]; //update pump
								n_cars_in_gasoline_queue= n_cars_in_gasoline_queue -1;
								
								
								/*delete front from queue*/
								front_gasoline = (front_gasoline + 1) % 8; //dequeue
								is_gasoline_queue_not_full=1; //queue not full
								pump_status[i]=0; //busy
								azaltma[i]=1;
							end
						end
						
					end
				
				
				if(fuel_type==0)begin //gas
					if(fuel_amount>4'd8 || fuel_amount==4'd0 || is_gasoline_queue_not_full==0)begin
						invalid_gasoline_car=1;
					end
					else begin //hata yok bas
						if(!(is_gasoline_queue_not_full==1 && ((back_gasoline+1)%8 == front_gasoline)) && is_gasoline_queue_not_full==1)begin //queue is not empty and not full
							back_gasoline = (back_gasoline +1 )%8 ;
							gas_queue[back_gasoline] =fuel_amount;
							
							n_cars_in_gasoline_queue=n_cars_in_gasoline_queue+1;
							total_gasoline_needed = total_gasoline_needed + fuel_amount;
							
							if((back_gasoline+1)%8 == front_gasoline)begin
								is_gasoline_queue_not_full = 0;
							end
						end//queue ye ekle sonu
						else if(is_gasoline_queue_not_full==1 && ((back_gasoline+1)%8 == front_gasoline))begin //queue empty
							added=0;
							for(i=0;i<6;i=i+1)begin
								if(i<n2_gasoline_pumps && added==0)begin
									if(pump_status[i]==1)begin
										added=1;
										pump_status[i]=0;
										pump_current[i]=fuel_amount;
										/*n_cars_in_gasoline_queue= n_cars_in_gasoline_queue +1;*/
										total_gasoline_needed = total_gasoline_needed + fuel_amount;
										is_gasoline_queue_not_full=1;
										azaltma[i]=1;
									end
								end
							end
							
							if(added==0)begin //pompalar doluysa
								back_gasoline = (back_gasoline +1 )%8 ;
								gas_queue[back_gasoline] =fuel_amount;
								n_cars_in_gasoline_queue=n_cars_in_gasoline_queue+1;
								total_gasoline_needed = total_gasoline_needed + fuel_amount;
								if((back_gasoline+1)%8 == front_gasoline)begin
									is_gasoline_queue_not_full = 0;
								end
								
							end///pompa doluysa geri queue ye ekledi
							
							
						end //queue empty sonu
					end //hata yok sonu
					
				end//gas sonu
				
				
				else if(fuel_type==1)begin//dizel
					
					//simdilik bos
					invalid_diesel_car=1;
					
				end//dizel sonu
				
				/*--1 time sim bas---*/
				
			
				for(i=0;i<6;i=i+1)begin
					if(i<n2_gasoline_pumps)begin //iterate over used gasoline pumps
						if(pump_status[i]==0 && azaltma[i]==0)begin //if pump is in use
							total_gasoline_needed=total_gasoline_needed - 1; //decrement total
							pump_current[i]=pump_current[i] - 1;  //decrement that pump
						end
						
						if(pump_current[i]==0)begin //if complete
							pump_status[i]=1; //make pump available again
						end
					end
				end
				/*--1 time sim son*/
			
			azaltma=6'b000000;
			end//onlygas sonu
			
			else if(n2_gasoline_pumps==0)begin //only dizel
				
				
				azaltma=6'b000000;
				/* ---araclari boslara yonlendir--*/
				for(i=0;i<6;i=i+1)begin
					if(i<n2_diesel_pumps)begin
						if(pump_status[i]==1 && !( is_diesel_queue_not_full==1 &&(back_diesel+1)%8 == front_diesel))begin
							/*total_diesel_needed= total_diesel_needed + diesel_queue[front_diesel]; *///update
							pump_current[i]=diesel_queue[front_diesel];
							/*delete front from queue*/
							front_diesel = (front_diesel + 1) % 8; //diesel
							is_diesel_queue_not_full=1;
							pump_status[i]=0;
							azaltma[i]=1;
							
						end
					end
				end
				
				if(fuel_type==0)begin //gas
				// kalsin simdilik
				invalid_gasoline_car = 1;

				end
				else if(fuel_type==1)begin//diesel
					if(fuel_amount>4'd8 || fuel_amount==4'd0 || is_diesel_queue_not_full==0)begin
						invalid_diesel_car=1;
					end
					
					else begin //hata yok basi
						if(!(is_diesel_queue_not_full==1 && ((back_diesel+1)%8 == front_diesel)) && is_diesel_queue_not_full==1)begin //queue is not empty and not full
							back_diesel = (back_diesel +1 )%8 ;
							diesel_queue[back_diesel] =fuel_amount;
							
							n_cars_in_diesel_queue=n_cars_in_diesel_queue+1;
							total_diesel_needed = total_diesel_needed + fuel_amount;
							
							if((back_diesel+1)%8 == front_gasoline)begin
								is_gasoline_queue_not_full = 0;
							end
							
						end//queue ye ekle sonu
						
						else if(is_diesel_queue_not_full==1 && ((back_diesel+1)%8 == front_diesel))begin //queue empty
							added=0;
							for(i=0;i<6;i=i+1)begin
								if(i<n2_diesel_pumps && added==0)begin
									if(pump_status[i]==1)begin
										added=1;
										pump_status[i]=0;
										pump_current[i]=fuel_amount;
										/*n_cars_in_diesel_queue= n_cars_in_diesel_queue +1;*/
										total_diesel_needed = total_diesel_needed + fuel_amount;
										is_diesel_queue_not_full=1;
										azaltma[i]=1;
									end
								end
							end
							
							if(added==0)begin //pompalar doluysa
								back_diesel = (back_diesel +1 )%8 ;
								diesel_queue[back_diesel] =fuel_amount;
								n_cars_in_diesel_queue=n_cars_in_diesel_queue+1;
								total_diesel_needed = total_diesel_needed + fuel_amount;
								if((back_diesel+1)%8 == front_diesel)begin
									is_diesel_queue_not_full = 0;
								end
								
							end///pompa doluysa geri queue ye ekledi
							
							
						end //queue empty sonu
						
						
					end//hata yok sonu
				end
					
					/*1 time sim basi*/
					for(i=0;i<6;i=i+1)begin //iterate over used diesel pumps
						if(i<n2_diesel_pumps)begin
							if(pump_status[i]==0 && azaltma[i]==0)begin //if pump is in use
								total_diesel_needed=total_diesel_needed - 1; //decrement total
								pump_current[i]=pump_current[i] - 1;  //decrement that pump
								if(pump_current[i]==0)begin //if complete
									pump_status[i]=1; //make pump available again
								end
							end
						end
					end
					/*1 time sim sonu*/
					
				
			
			azaltma=6'b000000;
			end//only dizel sonu
			
			else begin //hem gaz hem dizel basi
				/*bos araclari yonlendir*/
				/*once gas*/
				azaltma=6'b000000;
				
				for(i=0;i<6;i=i+1)begin //first add gas if available
					if(i<n2_gasoline_pumps)begin
						if(pump_status[i]==1 && !( is_gasoline_queue_not_full==1 && (back_gasoline+1)%8 == front_gasoline))begin // if there is a car in queue
							/*total_gasoline_needed= total_gasoline_needed + gas_queue[front_gasoline];*/ //update
							pump_current[i]=gas_queue[front_gasoline];
							n_cars_in_gasoline_queue= n_cars_in_gasoline_queue - 1;
 							
							front_gasoline=(front_gasoline + 1)%8 ; //dequeue
							is_gasoline_queue_not_full = 1;
							pump_status[i]=0; //busy
							azaltma[i]=1;
						end
					end
				end
				/*sonra dizel*/
				for(i=0;i<6;i=i+1)begin //then add diesel if available
					if(i<n2_diesel_pumps)begin
						if(pump_status[i+n2_gasoline_pumps]==1 && !( is_diesel_queue_not_full==1 &&(back_diesel+1)%8 == front_diesel) )begin
							/*total_diesel_needed= total_diesel_needed + diesel_queue[front_diesel];*/ //update
							pump_current[i+n2_gasoline_pumps]=diesel_queue[front_diesel];
							n_cars_in_diesel_queue= n_cars_in_diesel_queue - 1;
							
							front_diesel=(front_diesel + 1)% 8; //dequeue
							is_diesel_queue_not_full=1;
							pump_status[i+n2_gasoline_pumps]=0; //busy
							azaltma[i+n2_gasoline_pumps]=1;
						end
					end
				end
				
				if(fuel_type==0)begin //gas basi
					/*------------------*/
					if(fuel_amount>4'd8 || fuel_amount==4'd0 || is_gasoline_queue_not_full==0)begin
						invalid_gasoline_car=1;
					end
					else begin //hata yok bas
						if(!(is_gasoline_queue_not_full==1 && ((back_gasoline+1)%8 == front_gasoline)) && is_gasoline_queue_not_full==1)begin //queue is not empty and not full
							back_gasoline = (back_gasoline +1 )%8 ;
							gas_queue[back_gasoline] =fuel_amount;
							
							n_cars_in_gasoline_queue=n_cars_in_gasoline_queue+1;
							total_gasoline_needed = total_gasoline_needed + fuel_amount;
							
							if((back_gasoline+1)%8 == front_gasoline)begin
								is_gasoline_queue_not_full = 0;
							end
						end//queue ye ekle sonu
						else if(is_gasoline_queue_not_full==1 && ((back_gasoline+1)%8 == front_gasoline))begin //queue empty
							added=0;
							for(i=0;i<6;i=i+1)begin
								if(i<n2_gasoline_pumps &&added==0)begin
									if(pump_status[i]==1)begin
										added=1;
										pump_status[i]=0;
										pump_current[i]=fuel_amount;
										/*n_cars_in_gasoline_queue= n_cars_in_gasoline_queue +1;*/
										total_gasoline_needed = total_gasoline_needed + fuel_amount;
										is_gasoline_queue_not_full=1;
										azaltma[i]=1;
									end
								end
							end
							
							if(added==0)begin //pompalar doluysa
								back_gasoline = (back_gasoline +1 )%8 ;
								gas_queue[back_gasoline] =fuel_amount;
								n_cars_in_gasoline_queue=n_cars_in_gasoline_queue+1;
								total_gasoline_needed = total_gasoline_needed + fuel_amount;
								if((back_gasoline+1)%8 == front_gasoline)begin
									is_gasoline_queue_not_full = 0;
								end
								
							end///pompa doluysa geri queue ye ekledi
							
							
						end //queue empty sonu
					end //hata yok sonu
					/*------------------*/
					
				end//gas sonu
				else if(fuel_type==1)begin//dizel basi
				
				/*------------------*/
					if(fuel_amount>4'd8 || fuel_amount==4'd0 || is_diesel_queue_not_full==0)begin
						invalid_diesel_car=1;
					end
					
					else begin //hata yok basi
						if((is_diesel_queue_not_full==1) && !(((back_diesel+1)%8 == front_diesel) && is_diesel_queue_not_full==1))begin //queue is not empty and not full
							back_diesel = (back_diesel +1 )%8 ;
							diesel_queue[back_diesel] =fuel_amount;
							
							n_cars_in_diesel_queue=n_cars_in_diesel_queue+1;
							total_diesel_needed = total_diesel_needed + fuel_amount;
							
							if((back_diesel+1)%8 == front_diesel)begin
								is_diesel_queue_not_full = 0;
							end
							
						end//queue ye ekle sonu
						
						else if(is_diesel_queue_not_full==1 && ((back_diesel+1)%8 == front_diesel))begin //queue empty
							added=0;
							for(i=0;i<6;i=i+1)begin
								if(i<n2_diesel_pumps)begin
									if(pump_status[i+n2_gasoline_pumps]==1 && added==0)begin
										added=1;
										pump_status[i+n2_gasoline_pumps]=0;
										pump_current[i+n2_gasoline_pumps]=fuel_amount;
										/*n_cars_in_diesel_queue= n_cars_in_diesel_queue +1;*/
										total_diesel_needed = total_diesel_needed + fuel_amount;
										is_diesel_queue_not_full=1;
										azaltma[i+n2_gasoline_pumps]=1;
									end
								end
							end
							
							if(added==0)begin //pompalar doluysa
								back_diesel = (back_diesel +1 )%8 ;
								diesel_queue[back_diesel] =fuel_amount;
								n_cars_in_diesel_queue=n_cars_in_diesel_queue+1;
								total_diesel_needed = total_diesel_needed + fuel_amount;
								if((back_diesel+1)%8 == front_diesel)begin
									is_diesel_queue_not_full = 0;
								end
								
							end///pompa doluysa geri queue ye ekledi
							
							
						end //queue empty sonu
						
						
					end//hata yok sonu
					/*------------------*/
					
					
				
				end//dizel sonu
				
			/*---------1 time simulate---------*/
			/*simulate for 1 time*/
				for(i=0;i<6;i=i+1)begin //iterate over used gasoline pumps
					if(i<n2_gasoline_pumps)begin
						if(pump_status[i]==0 && azaltma[i]==0)begin //if pump is in use
							total_gasoline_needed=total_gasoline_needed - 1; //decrement total
							pump_current[i]=pump_current[i] - 1;  //decrement that pump
							
							if(pump_current[i]==0)begin //if complete
								pump_status[i]=1; //make pump available again
							end
						end
					end
				end
				
				for(i=0;i<6;i=i+1)begin //iterate over used diesel pumps
					if(i<n2_diesel_pumps)begin
						if(pump_status[i+n2_gasoline_pumps]==0 && azaltma[i+n2_gasoline_pumps]==0)begin //if pump is in use
							total_diesel_needed=total_diesel_needed - 1; //decrement total
							pump_current[i+n2_gasoline_pumps]=pump_current[i+n2_gasoline_pumps] - 1;  //decrement that pump
							
							if(pump_current[i+n2_gasoline_pumps]==0)begin //if complete
								pump_status[i+n2_gasoline_pumps]=1; //make pump available again
							end
						end
					end
				end
			/*---------1 time simulate---------*/
				
				azaltma=6'b000000;
			end//hem gaz hem dizel sonu
		end //addcar sonu
end
endmodule
