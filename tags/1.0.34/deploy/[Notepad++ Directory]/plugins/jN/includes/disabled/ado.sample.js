var cnn = new ActiveXObject("ADODB.Connection");
cnn.Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=g:\\PLZ_Bundesland.mdb;");
//cnn.Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=g:\\mappe1.xls;Extended Properties=Excel 8.0;");
var tb = cnn.OpenSchema(20);
tb.MoveFirst();
var arr = [];
try{
	while (!tb.EOF){
		var tableName = tb.Fields(2).Value;
		var records =0;
		try{
			var rs = cnn.Execute("SELECT count(*) FROM `"+tableName+"`");
			records = rs.Fields(0).Value;
		}catch(e){
			records = e.description;
		}
		arr.push(tableName+ "  "+records);
		tb.MoveNext();
	}
}finally{
	cnn.Close();
}

MenuCmds.FILE_NEW();
currentView.text = arr.join('\n');
