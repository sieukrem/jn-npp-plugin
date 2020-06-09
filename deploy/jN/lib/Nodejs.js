process = new function() {
	this.cwd = function () { return shell.CurrentDirectory; }	
	this.argv = [];
	/*
	for (i = 0; i < WScript.Arguments.length; i++)
	{
		this.argv.push(WScript.Arguments(i));
	}*/
	
	var objWMIService = GetObject("winmgmts:\\\\.\\root\\cimv2");
	
	this.env = GetEnvironmentVariables();
	this.pid = GetCurrentPID();
	
	
	function GetEnvironmentVariables(){
		var colVar = new Enumerator(objWMIService.ExecQuery("Select * from Win32_Environment"))
		var env = {};
		for (; !colVar.atEnd(); colVar.moveNext())
		{
			var item = colVar.item();
			env[item.Name] = item.VariableValue;
		}
		return env;
	}
	
	function GetCurrentPID(){
		var childProcess = shell.Exec("%comspec% /C echo text && set /p var=");

		childProcess.StdOut.ReadLine(); // waits until the process started
			
        var result = new Enumerator(objWMIService.ExecQuery( "Select * From Win32_Process Where ProcessId="+childProcess.ProcessID )).item().ParentProcessId;
		
		childProcess.StdIn.WriteLine("text"); // waits until the parentProcessId was determined
		
		return result;
	}
}