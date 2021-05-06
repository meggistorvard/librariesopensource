(function ($) {

/**
* Display offline/online connection status in Javascript using Offline.js. 
* Offline.js is a library to automatically alert your users when they've 
* lost internet connectivity
*/
Gradiscake.behaviors.isOffline = { 
attach: function(context)  {
	
	Offline.options = {
	  // to check the connection status immediatly on page load.
	  checkOnLoad: false, 

	  // to monitor AJAX requests to check connection.
	  interceptRequests: true,

	  // to automatically retest periodically when the connection is down (set to false to disable).
	  reconnect: {
		// delay time in seconds to wait before rechecking.
		initialDelay: 3,

		// wait time in seconds between retries.
		delay: 10
	  },

	  // to store and attempt to remake requests which failed while the connection was down.
	  requests: true
	  
	};
}
}

})(jQuery);
