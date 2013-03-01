var wmpmenu = new (function(){
	var self = this;
	self.wmp_source = GlobalSettings.get("WMP_SOURCE") || "mms://channels.webradio.antenne.de:80/top-40";

	this.GetPlayer = function(){
		if (!this.player)
			this.player = new ActiveXObject("WMPlayer.OCX");
		
		this.player.controls.stop();

		return this.player;
	}
	
	var wmp = addMenu({
		text: "WMP",
		oninitpopup: function(){
			self.play.disabled = self.player && (self.player.playState == 3 || self.player.playState == 9);
			self.stop.disabled = !self.player || self.player.playState == 1 || self.player.playState == 9; 
			self.pause.disabled = !self.player || self.player.playState == 1 || self.player.playState == 9; 
			
			if (!self.player)
				return;
				
			self.status.text = self.player.status;
		}
	});

	this.play = wmp.addItem({
		text: "Play",
		cmd: function(){
			self.GetPlayer().URL = self.wmp_source;
			self.GetPlayer().controls.play();
		}
	});

	this.stop = wmp.addItem({
		text: "Stop",
		cmd: function(){
			self.GetPlayer().controls.stop();
		}
	});

	this.pause = wmp.addItem({
		text: "Pause",
		cmd: function(){
			self.GetPlayer().controls.pause();
		}
	});

	wmp.addSeparator();
	this.source = wmp.addItem({
		text: "Set Media Source...",
		cmd:function(){
			Dialog.prompt("Set Media Source", self.wmp_source, function(value){
				self.wmp_source = value;
			
				GlobalSettings.set("WMP_SOURCE", value);
				
				self.GetPlayer().URL = self.wmp_source;
				self.GetPlayer().controls.play();
			});
		}
	});
	
	wmp.addSeparator();
	this.status = wmp.addItem({
		text: "Waiting..."
	});
	this.status.disabled = true;
	
})();