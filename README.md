// set up possible commands. 
command help = "help" => "list all available commands ";
command help cmd= "help cmd" => "help for specified command,cmd is replaced with the available
                                 commands.e.g.: help min ";
command min = "min product ask|bid" => "find the minimum bid or ask for the product in the current time step";
command max ="max product ask|bid" => "find the maximum bid or ask for the product in the current time step";
command avg = "avg product ask|bid timesteps" => "computes the average ask or bid for the sent product
                                                   over the sent number of time steps";
command predict = "predict max|min product ask|bid" => "predict max or min ask or bid for the sentproduct for
                                                        the next time step using the moving average";
command time = "time" => "prints the current time in the simulated dataset";
command step = "step" => "move to the next time step in the simulated dataset";
command back = "back" => "move to the previous time step in the simulated dataset";
command exit = "exit" => "exit the program with a success status code";
