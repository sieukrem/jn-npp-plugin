        var objShell = new ActiveXObject("Shell.Application");
        var objShellWindows;
        
        objShellWindows = objShell.Windows();
        if (objShellWindows != null)
        {
            var objIE;
            objIE = objShellWindows.Item(0);

            if (objIE != null)
            {
				// alert(objIE.StatusText);
				objIE.StatusText = "blah blah";
				
				var ShellFolderView = objIE.Document;
				
				alert(ShellFolderView.Folder.Items().Count);
				ShellFolderView.SelectItem(ShellFolderView.Folder.Items().Item(1),3);
				
				alert(ShellFolderView.Folder.Items().Item(0).Path);
				
				ShellFolderView.PopupItemMenu(ShellFolderView.Folder.Items().Item(1));
            }
        }
