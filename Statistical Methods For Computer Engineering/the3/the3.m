N=4144;  %size of the Monte Carlo simulation with alpha = 0.01 and error = 0.02
lambda_motor = 40; %number of motor pass over the bridge
lambda_auto = 30; %number of automobile pass over the bridge
lambda_truck = 20; %number of trucks pass over the bridge
TotalWeight =zeros(N,1); %vector to keep the total weight for each run
for k=1:N;
    U1 = rand; i1 = 0;
    U2 = rand; i2 = 0;
    U3 = rand; i3 = 0;

    F = exp(-lambda_motor);
    while(U1>=F);
        i1=i1+1;
        F = F+exp(-lambda_motor)*lambda_motor^i1/gamma(i1+1);
        
    end;
    moto_num = i1; %moto_num is the number of motorcycle passed

    F = exp(-lambda_auto);
    while(U2>=F);
        i2=i2+1;
        F = F+exp(-lambda_auto)*lambda_auto^i2/gamma(i2+1);
        
    end;
    auto_num = i2; %auto_num is the number of automobiles passed

    F = exp(-lambda_truck);
    while(U3>=F);
        i3=i3+1;
        F = F+exp(-lambda_truck)*lambda_truck^i3/gamma(i3+1);
        
    end;
    truck_num = i3; %truck_num is the number of motorcycle passed


    lambda_motor_gamma = 0.15;
    alpha_motor = 16;
    lambda_auto_gamma = 0.05;
    alpha_auto = 60;
    lambda_truck_gamma = 0.01;
    alpha_truck = 84;




     
    X1 = sum( -1/lambda_motor_gamma * log(rand(alpha_motor,moto_num)));
    X2 = sum( -1/lambda_auto_gamma * log(rand(alpha_auto,auto_num)));
    X3 = sum( -1/lambda_truck_gamma * log(rand(alpha_truck,truck_num))); 
    

   







 

    TotalWeight(k) = sum(X1) + sum(X2) + sum(X3);
 






end;

p_est = mean(TotalWeight>220000);
expectedWeight = mean(TotalWeight);
stdWeight = std(TotalWeight);
fprintf('Estimated probability = %f\n',p_est);
fprintf('Expected weight = %f\n',expectedWeight);
fprintf('Standard deviation = %f\n',stdWeight);


 %{
  This commented code is another approach for calculating weights.But takes more time.
    cx=0;
    X1=0;
    X2=0;
    X3=0;
    for count1=1:moto_num;
        
        cx=sum(-1/lambda_motor_gamma * log(rand(alpha_motor,1)));
        X1=X1+cx;
    end;

    X2=0;
    for count2=1:auto_num;
        cx=sum(-1/lambda_auto_gamma * log(rand(alpha_auto,1)));
        X2=X2+cx;
    end;

    X3=0;
    for count3=1:truck_num;
        cx=sum(-1/lambda_truck_gamma * log(rand(alpha_truck,1)));
        X3=X3+cx;
    end;

    TotalWeight(k)=X1+X2+X3; 
  
%}


