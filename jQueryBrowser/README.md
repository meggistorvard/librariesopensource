
[![NPM](https://nodei.co/npm/jquery.browser.png)](https://nodei.co/npm/jquery.browser/)

A jQuery plugin for browser detection. jQuery v1.9.1 dropped support for browser detection, and this project aims to keep the detection up-to-date.

Installation
=======================

Include script *after* the jQuery library:
```html
<head>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.1/jquery.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery-browser/0.1.0/jquery.browser.min.js"></script>
  <script src="/path/to/jquery.browser.js"></script>
</head>

or 
<head>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery-browser/0.1.0/jquery.browser.min.js"></script>
</head>

console.log($.browser
```

Alternatively, you can use the plugin without jQuery by using the global object `jQBrowser` instead of `$.browser`.


Usage
=======================

Returns true if the current useragent is some version of Microsoft's Internet Explorer. Supports all IE versions including IE 11.

    $.browser.msie;

Returns true if the current useragent is some version of a WebKit browser (Safari, Chrome and Opera 15+)

    $.browser.webkit;

Returns true if the current useragent is some version of Firefox

    $.browser.mozilla;

Reading the browser version

    $.browser.version

You can also examine arbitrary useragents

    jQBrowser.uaMatch();

	
## Things not included in the original jQuery $.browser implementation

- Detect specifically Windows, Mac, Linux, iPad, iPhone, iPod, Android, Kindle, BlackBerry, Chrome OS, and Windows Phone useragents

```javascript
	$.browser.android
	$.browser.blackberry
	$.browser.cros
	$.browser.ipad
	$.browser.iphone
	$.browser.ipod
	$.browser.kindle
	$.browser.linux
	$.browser.mac
	$.browser.msedge
	$.browser.playbook
	$.browser.silk
	$.browser.win
	$.browser["windows phone"]
```

Alternatively, you can detect for generic classifications such as desktop or mobile

```javascript
	$.browser.desktop
	$.browser.mobile
```

```javascript
	// User Agent for Firefox on Windows
	User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:25.0) Gecko/20100101 Firefox/25.0
	
	$.browser.desktop // Returns true as a boolean
```

```javascript
	// User Agent for Safari on iPhone
	User-Agent: Mozilla/5.0(iPhone; CPU iPhone OS 7_0_3 like Mac OS X; en-us) AppleWebKit/537.51.1 (KHTML, like Gecko) Version/7.0 Mobile/11B508 Safari/9537.53
	
	$.browser.mobile // Returns true as a boolean
```

- Detect the browser's major version

```javascript
	// User Agent for Chrome
	// Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/32.0.1664.3 Safari/537.36
	
	$.browser.versionNumber // Returns 32 as a number
```

- Support for new useragent on IE 11
- Support for Microsoft Edge
- Support for WebKit based Opera browsers
- Added testing using PhantomJS and different browser user agents


Sample
=======================

### Example: Returns the browser version.

```html
<!DOCTYPE html>
<html>
<head>
  <style>
  p { color:blue; margin:20px; }
  span { color:red; }
  </style>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery-browser/0.1.0/jquery.browser.min.js"></script>
</head>
<body>
	<p>
  </p>
<script>

    $("p").html("The browser version is: <span>" +
                jQuery.browser.version + "</span>");
</script>
</body>
</html>
```

### Example: Alerts the version of IE that is being used

```javascript
  if ( $.browser.msie ) {
    alert( $.browser.version );
  }
  if (jQuery.browser.msie) {
    alert(parseInt(jQuery.browser.version));
  }
```

Here are some typical results:

```
  Internet Explorer: 6.0, 7.0
  Mozilla/Firefox/Flock/Camino: 1.7.12, 1.8.1.3, 1.9
  Opera: 9.20
  Safari/Webkit: 312.8, 418.9
```

Note that IE8 claims to be 7 in Compatibility View.


Code of Conduct
=======================

#### Why do I need a code of conduct?

A code of conduct is a document that establishes expectations for behavior for your project’s participants. Adopting, and enforcing, a code of conduct can help create a positive social atmosphere for your community.
By participating, you are expected to honor this code.


License
=======================

## MIT Open Source License

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[MIT License](https://opensource.org/licenses/MIT)

Donations
=======================
## Donate to Meggis and help us improve free software

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
