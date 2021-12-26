## README

### Important 
- This work uses Python3
- Tested on Linux/Ubuntu
- This work uses relative paths to current directory. ( ex: "./savedmodels" )
    - Therefore if you have some problems with saving or loading, you may need to arrange your pythonpath.


### Required python packages: 
- Pytorch
- Matplotlib
- Numpy
- PIL
- torchvision
- os (default library)

### Installation of the environment:
- Install required packages for python3
- Extract the1.zip
- Put "data" folder in the same directory where train.py resides.
   - Check "data" folder contains *train* and *test* folders. 
	    *(Ensure that train and test folders contain labels.txt)*
- Under directory, create two folders with exact these names:
     - savedmodels 
     -  plots
     *(This is necessary, since the code saves plots and model_state_dict's for every model)*
     
 
 - Run command `python train.py` on the terminal. This will print all the logs to terminal.
- To save training logs, you can direct stdout to .txt file 
    - ex:  `python train.py > output.txt`
- When the code finishes running, it will select best model to test on test set.
     - It will load the best model from savedmodels folder 
    -  It will create 'inference_results.txt in the same directory where you are running the script.
    - This inference_results.txt can be uploaded to website to check accuracy.
    - You can reach plots and savedmodels under the related folders.
		- Example formatting for file savedmodel and plot names:
		    - `e_21_acc_none_hid_256_lr_0.05_1layer.png`
		   - e\_<finished_epoch>\_acc\_<used_activation>\_hid\_<hidden_size>\_lr\_<learning_rate>\_ <howmanylayer\>.png




