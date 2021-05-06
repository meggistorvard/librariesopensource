# Powerfull libraries Open-source for your website

## Open Source for More Flexibility

Display online/offline connection status in JavaScript using Offline.js
by Sumit Kumar Pradhan on March 17, 2018 March 17, 2018 in AJAX, CSS, html, html5, javascript, jquery, web development
In this tutorial we are going to learn how to display offline/online connection status in Javascript using Offline.js. Offline.js is a library to automatically alert your users when they've lost internet connectivity, like Gmail,Facebook.

offline.js alternative, offline.js not working, offline.js angular, offline js react, offline.js angular 4, using offline js, offline js simulate, install offline js

The best part is that it captures AJAX requests which were made while the connection was down, and remakes them when it's back up, so your app reacts perfectly.

Detect Internet Connection with Offline.js
You can easily integrate this library in your project by following below steps :
### Step -1 :
Include the offline.js script in the page.
```
<script src="offline.min.js"></script>
```

### Step - 2 :
Include one of the theme in the page.
```
<link rel="stylesheet" type="text/css" href="offline-theme-default.css">
```

### Step - 3 :
Include one of the language in the page.
```
<link rel="stylesheet" href="offline-language-english.min.css" />
```

### Step -4 :
To configure Offline.js we assign the following settings to the Offline.options property.
```
<script type="text/javascript">
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
</script>
```

In order to check internet connection using this offline.js library, you have to follow below steps one by one and check you observation.

First You need to disconnect the internet connection , after that it will show message "Connection Lost Reconnecting"
Second you need to connect the internet connection, after that it will show message "Your device is connected to internet"


## Demo Link : 
https://skptricks.github.io/learncoding/Check%20internet%20connection%20using%20offline%20js/index.html

## Download Link :
https://github.com/meggistorvard/offline/blob/master/offline.min.js


This is all about detecting internet connection with offline.js. Thank you for reading this article, and if you have any problem, have a another better useful solution about this article, please write message in the comment section.

## License

 MIT license, which puts almost no restrictions on how you can use it. Read the license text
 <a href="https://fullcalendar.io/license/" target="_blank">`https://fullcalendar.io/license/`</a>.

## Donations (Optional)

Donate to Meggis and help us improve free software
There are a number of ways in which you can support us and help make a huge difference to our charity whether it be financially, with your time or donating equipment! 

WAYS YOU CAN SUPPORT US
	Make a donation
	Donate software & hardware
	Recommend a game
	Provide keys
	Volunteer
	Follow our stream on twitch
	Fundraising ideas
	Become an ambassador

Supporting further development	
 <a href="https://www.techsupport.gradiscake.com/?q=donations-free-software" target="_blank">https://www.techsupport.gradiscake.com/?q=donations-free-software</a>
