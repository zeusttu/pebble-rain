var location = '';
var rain = '';
var longTerm = false;
var hours24 = false;
var busy = false;
var currentPointer = 0;
var currentAmount = 0;
var currentPos = 0;
var getCityAfterEvrything = false;

var rainDrops = [];
var rainTimes = [];
var rainPerHour = [];
var dates = [];
var temps = [];
var ampms = [];

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  if(!longTerm) {
    if(localStorage.getItem(0)) {
      switch (localStorage.getItem(0)) {
        case 'auto':
          localStorage.setItem(1, '3494c9cfa9ad9c655be645a40f860b3a');
          getCity(pos, true);
        break;
        case 'forecast':
          getRainFromForecast(pos, true);
        break;
        case 'forecast1h':
          getRainFromForecast(pos, true);
        break;
        case 'buienradar':
          getRainFromBuienradar(pos, true);
        break;
        case 'openweathermap':
          getRainOpenWeatherMap(pos, true);
        break;
        default:
          getCity(pos, true);
        break;
      }
    } else {
      getCity(pos, true);
    }
  }else{
    getRainOpenWeatherMap(pos, true);
  }
};

function getRainOpenWeatherMap(pos, andGetCity) {
  console.log('calling open weather map');

  // Units
  var units = 'metric'; 
  if(localStorage.getItem(5) && localStorage.getItem(5) == 'f') {
    units = 'imperial'; 
  } 

  // Construct URL
  var url = "http://api.openweathermap.org/data/2.5/forecast?lat=" + pos.coords.latitude + "&lon=" + pos.coords.longitude + "&APPID=a38b82649166fdfc7eff5adb8a03290d&units=" + units;

  // Send request to forecast
  xhrRequest(url, 'GET', 
    function(response) {

      response = JSON.parse(response);

      rainDrops = [];
      rainTimes = [];
      rainPerHour = [];
      temps = [];
      dates = [];
      ampms = [];

      var volume = 'mm/h';

      var rainArray = response.list ? response.list : [];

      if (rainArray.length) {
        for (var i = 0; i < 25; i++) {
          var date = new Date(rainArray[i].dt*1000);
          var minutes = "0" + date.getMinutes();
          var hours = "0" + date.getHours();
          var day = date.getDate();
          var month = date.getMonth() + 1;
          var formattedTime = hours.substr(-2) + ':' + minutes.substr(-2);
          var formattedDate = day + '/' + month;
          if (units == 'imperial') {
            formattedDate = month + '/' + day;
          }

          var drops = 0;
          var dropsPerHour = 0;
          
          if (rainArray[i].snow && rainArray[i].snow['3h']) {
            dropsPerHour = rainArray[i].snow['3h'];
          }

          if (rainArray[i].rain && rainArray[i].rain['3h']) {
            dropsPerHour = rainArray[i].rain['3h'];
          }

          var dropsPerHourForDrops = dropsPerHour;
          if(dropsPerHourForDrops && dropsPerHourForDrops < 0.2) dropsPerHourForDrops = 0.2;
          drops = Math.round((dropsPerHourForDrops / 3.5) * 255);
          
          if (drops > 255) drops = 255;

          var temp = Math.round(parseInt(rainArray[i].main.temp));
          if (units == 'imperial') {
            temp += '°F';
          }else {
            temp += '°C';
          }

          var ampm = 0;
          if (!hours24) {
            var hours = parseInt(formattedTime.substr(0, 2));
            var minutes = formattedTime.substr(3, 2);

            if (hours < 12) {
              ampm = 1;
            } else {
              ampm = 2;
            }

            if (hours == 0) {
              hours = 12;
            }

            if (hours > 12) {
              hours = hours - 12;
            }

            hours = "0" + hours;

            formattedTime = hours.substr(-2) + ':' + minutes;
          }

          if(localStorage.getItem(7) == 'inch') {
            volume = 'in/h';
            dropsPerHour = (dropsPerHour/25.4).toFixed(4);
            if (dropsPerHour == '0.000' || dropsPerHour < 0.001) {
              dropsPerHour = '0.001';
            }
          }

          dates[i] = formattedDate;
          temps[i] = temp;
          rainTimes[i] = formattedTime;
          rainPerHour[i] = dropsPerHour;
          rainDrops[i] = drops;
          ampms[i] = ampm;
        }

        var refresh = 5;
        if (localStorage.getItem(6)) refresh = parseInt(localStorage.getItem(6));

        busy = true;
        currentPointer = 0;
        currentAmount = rainDrops.length;
        getCityAfterEvrything = andGetCity;
        currentPos = pos; 

        // Assemble dictionary using our keys
        var dictionary = {
          "KEY_READY": null,
          "KEY_LOCATION": null,
          "KEY_RAINDROPS": null,
          "KEY_RAINTIMES": null,
          "KEY_RAINPERHOUR": null,
          "KEY_RAINUNIT": volume,
          "KEY_NOW": 0,
          "KEY_REFRESH": null,
          "KEY_TEMP": null,
          "KEY_DATE": null,
          "KEY_LONGTERM": currentAmount,
          "KEY_24H": null
        };

        // Send to Pebble
        Pebble.sendAppMessage(dictionary,
          function(e) {
            console.log("Rain info sent to Pebble successfully!");
            sendNextData();
          },
          function(e) {
            console.log("Error sending rain info to Pebble!");
          }
        );
      } else {
        console.log('get rain from forecast')
        if(!localStorage.setItem(1)) {
          localStorage.setItem(1, '3494c9cfa9ad9c655be645a40f860b3a');
        }
        getRainFromForecast(pos, andGetCity);
      }

    }      
  );

};

function getRainFromForecast(pos, andGetCity) {
  console.log('calling forecast');

  // Units
  var units = 'metric'; 
  if(localStorage.getItem(5) && localStorage.getItem(5) == 'f') {
    units = 'imperial'; 
  } 

  // APIKEY
  var apikey = localStorage.getItem(1);//'484a2a4d5082e5bda98a7070cd735771';

  // Construct URL
  var url = "https://api.forecast.io/forecast/" + apikey + "/" + pos.coords.latitude + "," + pos.coords.longitude;
  //var url = "https://api.forecast.io/forecast/" + apikey + "/" + "52.627844,1.299552"; // Norwich, UK

  // Send request to forecast
  xhrRequest(url, 'GET', 
    function(response) {

      response = JSON.parse(response);

      rainDrops = [];
      rainTimes = [];
      rainPerHour = [];
      dates = [];
      temps = [];
      ampms = [];

      var rainArray = [];
      var maxLength = 0;

      var temperature = null;
      var volume = 'in/h';

      if (response.daily) {
        rainArray = response.daily.data;
        maxLength = rainArray.length;
        temperature = response.daily.data[0].temperature;
      }
      if (response.hourly) {
        rainArray = response.hourly.data;
        maxLength = 25;
        temperature = response.hourly.data[0].temperature;
      }
      if (response.minutely) {
        rainArray = response.minutely.data;
        maxLength = 61;
      }

      if (rainArray.length < maxLength) maxLength = rainArray.length;

      if (rainArray.length) {
        for (var i = 0; i < maxLength; i++) {
          var date = new Date(rainArray[i].time*1000);
          var minutes = "0" + date.getMinutes();
          var hours = "0" + date.getHours();
          var day = date.getDate();
          var month = date.getMonth() + 1;
          var formattedTime = hours.substr(-2) + ':' + minutes.substr(-2);
          var formattedDate = day + '/' + month;
          if (units == 'imperial') {
            formattedDate = month + '/' + day;
          }

          var intesityForDrops = rainArray[i].precipIntensity;
          if(intesityForDrops && intesityForDrops < 0.01) intesityForDrops = 0.01;
          var drops = Math.round((intesityForDrops / 0.25) * 255);
          if (drops > 255) drops = 255;

          if(rainArray[i].temperature) temperature = rainArray[i].temperature;

          var displayTemperature = ' ';
          if(localStorage.getItem(5) == 'f') {
            displayTemperature = Math.round(temperature)+'°F';
          }else{
            displayTemperature = Math.round((temperature - 32) * (5 / 9)) +'°C';
          }

          var ampm = 0;
          if (!hours24) {
            var hours = parseInt(formattedTime.substr(0, 2));
            var minutes = formattedTime.substr(3, 2);

            if (hours < 12) {
              ampm = 1;
            } else {
              ampm = 2;
            }

            if (hours == 0) {
              hours = 12;
            }

            if (hours > 12) {
              hours = hours - 12;
            }

            hours = "0" + hours;

            formattedTime = hours.substr(-2) + ':' + minutes;
          }

          var dropsPerHour = rainArray[i].precipIntensity;

          if(localStorage.getItem(7) == 'mm') {
            volume = 'mm/h';
            dropsPerHour = (dropsPerHour*25.4).toFixed(4);
            if (dropsPerHour == '0.000' || dropsPerHour < 0.001) {
              dropsPerHour = '0.001';
            }
          }

          rainTimes[i] = formattedTime;
          rainPerHour[i] = dropsPerHour;
          rainDrops[i] = drops;
          dates[i] = formattedDate;
          temps[i] = displayTemperature;
          ampms[i] = ampm;
        }

        var refresh = 5;
        if (localStorage.getItem(6)) refresh = parseInt(localStorage.getItem(6));

        busy = true;
        currentPointer = 0;
        currentAmount = rainDrops.length;
        getCityAfterEvrything = andGetCity;
        currentPos = pos; 

        // Assemble dictionary using our keys
        var dictionary = {
          "KEY_READY": null,
          "KEY_LOCATION": null,
          "KEY_RAINDROPS": null,
          "KEY_RAINTIMES": null,
          "KEY_RAINPERHOUR": null,
          "KEY_RAINUNIT": volume,
          "KEY_NOW": 0,
          "KEY_REFRESH": refresh,
          "KEY_TEMP": null,
          "KEY_DATE": null,
          "KEY_LONGTERM": currentAmount,
          "KEY_24H": null
        };

        // Send to Pebble
        Pebble.sendAppMessage(dictionary,
          function(e) {
            console.log("Rain info sent to Pebble successfully!");
            sendNextData();
          },
          function(e) {
            console.log("Error sending rain info to Pebble!");
          }
        );
      }

    }      
  );

};

function getRainFromBuienradar(pos, andGetCity) {
  // Units
  var units = 'metric'; 
  if(localStorage.getItem(5) && localStorage.getItem(5) == 'f') {
    units = 'imperial'; 
  } 

  // Construct URL
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" + pos.coords.latitude + "&lon=" + pos.coords.longitude + "&APPID=a38b82649166fdfc7eff5adb8a03290d&units=" + units;

  // Send request to forecast
  xhrRequest(url, 'GET', 
    function(response) {

      response = JSON.parse(response);
      var temp = '';

      if(response && response.main && response.main.temp) {
        temp = parseInt(response.main.temp);
        if (units == 'imperial') {
          temp += '°F';
        }else {
          temp += '°C';
        }
      }

      console.log('calling buienradar');

      // Construct URL
      var url = "http://gps.buienradar.nl/getrr.php?" +
          "lat=" + pos.coords.latitude + "&lon=" + pos.coords.longitude;

      // Send request to buienradar
      xhrRequest(url, 'GET', 
        function(responseText) {

          var rainString = responseText;
          rainString = rainString.replace(/\s/g, ';')
          rainString = rainString.replace(/;;/g, ';')

          if(rainString[rainString.length-1] === ';') {
            rainString = rainString.substr(0, rainString.length-1)
          }

          var rainArray = rainString.split(';');

          rainDrops = [];
          rainTimes = [];
          rainPerHour = [];
          dates = [];
          temps = [];
          ampms = [];

          var volume = 'mm/h';

          //rainDrops = [70,70,80,220,180,50,40,40,70,60,60,60,50,30,30,20,20,10,0,0,0,0,0]; // fake data

          for (var i = 0; i < rainArray.length; i++) {
            var data = rainArray[i].split('|');
            var drops = parseInt(data[0]);
            var time = data[1];

            var ampm = 0;
            if (!hours24) {
              var hours = parseInt(time.substr(0, 2));
              var minutes = time.substr(3, 2);

              if (hours < 12) {
                ampm = 1;
              } else {
                ampm = 2;
              }

              if (hours == 0) {
                hours = 12;
              }

              if (hours > 12) {
                hours = hours - 12;
              }

              hours = "0" + hours;

              time = hours.substr(-2) + ':' + minutes;
            }

            rainTimes[i] = time;
            ampms[i] = ampm;
            //rainDrops[i] = 10*i; // fake data
            //rainDrops[i] = drops;

            var perHour = Math.pow(10, (drops - 109)/32);
            if (perHour < 0.0005) {
              perHour = 0;
            } else if (perHour < 0.01) {
              perHour = Math.round(perHour*10000)/10000;
            } else if (perHour < 0.1) {
              perHour = Math.round(perHour*1000)/1000;
            } else if (perHour < 10) {
              perHour = Math.round(perHour*100)/100;
            } else {
              perHour = Math.round(perHour);
            }

            var dropsPerHourForDrops = perHour;
            if(dropsPerHourForDrops && dropsPerHourForDrops < 0.1) dropsPerHourForDrops = 0.1;
            newDrops = Math.round((dropsPerHourForDrops / 2) * 255);
            
            if (newDrops > 255) newDrops = 255;
            rainDrops[i] = newDrops;

            if(localStorage.getItem(7) == 'inch') {
              volume = 'in/h';
              perHour = (perHour/25.4).toFixed(4);
              if (perHour == '0.000' || perHour < 0.001) {
                perHour = '0.001';
              }
            }
            
            rainPerHour[i] = perHour;

            temps[i] = temp;
            dates[i] = ' ';
          }

          var refresh = 5;
          if (localStorage.getItem(6)) refresh = parseInt(localStorage.getItem(6));

          busy = true;
          currentPointer = 0;
          currentAmount = rainDrops.length;
          getCityAfterEvrything = andGetCity;
          currentPos = pos; 

          // Assemble dictionary using our keys
          var dictionary = {
            "KEY_READY": null,
            "KEY_LOCATION": null,
            "KEY_RAINDROPS": null,
            "KEY_RAINTIMES": null,
            "KEY_RAINPERHOUR": null,
            "KEY_RAINUNIT": volume,
            "KEY_NOW": 2,
            "KEY_REFRESH": null,
            "KEY_TEMP": null,
            "KEY_DATE": null,
            "KEY_LONGTERM": currentAmount,
            "KEY_24H": null
          };

          // Send to Pebble
          Pebble.sendAppMessage(dictionary,
            function(e) {
              console.log("Rain info sent to Pebble successfully!");
              sendNextData();
            },
            function(e) {
              console.log("Error sending rain info to Pebble!");
            }
          );
        }      
      );

    }      
  );
};

function getCity(pos, callApiFromCountry) {
  location = '';

  var lat = pos.coords.latitude;
  var lng = pos.coords.longitude;

  // Construct URL
  var url = "https://maps.googleapis.com/maps/api/geocode/json?latlng=" + lat + "," + lng;

  // Send request to google
  xhrRequest(url, 'GET', 
    function(responseText) {

      // responseText contains a JSON object with location info
      var json = JSON.parse(responseText);
      
      var closestDataComponents = json.results[0].address_components;
      var locationAttempt1 = '';
      var locationAttempt2 = '';
      var locationAttempt3 = '';
      var locationAttempt4 = '';
      var locationAttempt5 = '';
      var country = '';

      for (var i = 0; i < closestDataComponents.length; i++) {
        for (var j = 0; j < closestDataComponents[i].types.length; j++) {
          if(closestDataComponents[i].types[j] == "sublocality") locationAttempt1 = closestDataComponents[i].long_name;
          if(closestDataComponents[i].types[j] == "locality") locationAttempt2 = closestDataComponents[i].long_name;
          if(closestDataComponents[i].types[j] == "administrative_area_level_2") locationAttempt3 = closestDataComponents[i].long_name;
          if(closestDataComponents[i].types[j] == "administrative_area_level_1") locationAttempt4 = closestDataComponents[i].long_name;
          if(closestDataComponents[i].types[j] == "country") {
            locationAttempt5 = closestDataComponents[i].long_name;
            country = closestDataComponents[i].short_name;
          }
        }
      }

      location = locationAttempt1;
      if (!location) location = locationAttempt2;
      if (!location) location = locationAttempt3;
      if (!location) location = locationAttempt4;
      if (!location) location = locationAttempt5;
      if (!location) location = 'Unknown';


      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_READY": null,
        "KEY_LOCATION": location,
        "KEY_RAINDROPS": null,
        "KEY_RAINTIMES": null,
        "KEY_RAINPERHOUR": null,
        "KEY_RAINUNIT": null,
        "KEY_NOW": null,
        "KEY_REFRESH": null,
        "KEY_TEMP": null,
        "KEY_DATE": null,
        "KEY_LONGTERM": null,
        "KEY_24H": null
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Location info sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending rain info to Pebble!");
        }
      );

      // Call api if needed
      if(callApiFromCountry) {
        console.log('call api from country: ', country);
        switch (country) {
          case 'NL':
            getRainFromBuienradar(pos);
          break;
          case 'BE':
            getRainFromBuienradar(pos);
          break;
          case 'US':
            getRainFromForecast(pos);
          break;
          case 'GB':
            getRainFromForecast(pos);
          break;
          case 'IE':
            getRainFromForecast(pos);
          break;
          default:
            getRainOpenWeatherMap(pos);
          break;
        }
      }
    }      
  );
}

function locationError(err) {
  console.log("Error requesting location!");
}

function getLocation() {
  if(localStorage.getItem(2) === 'True' || localStorage.getItem(2) === 'true' || localStorage.getItem(2) === true || !localStorage.getItem(2)) {
    navigator.geolocation.getCurrentPosition(
      locationSuccess,
      locationError,
      {timeout: 15000, maximumAge: 60000}
    );
  }else{
    locationSuccess({
      coords: {
        latitude: parseFloat(localStorage.getItem(3)),
        longitude: parseFloat(localStorage.getItem(4))
      }
    })
  }
}

function sendNextData() {
  // Assemble dictionary using our keys

  //console.log(rainDrops[currentPointer] + ' / ' + rainTimes[currentPointer] + ' / ' + rainPerHour[currentPointer] + ' / ' + temps[currentPointer] + ' / ' + dates[currentPointer] + ' / ' + ampms[currentPointer])

  var dictionary = {
    "KEY_READY": null,
    "KEY_LOCATION": null,
    "KEY_RAINDROPS": rainDrops[currentPointer],
    "KEY_RAINTIMES": rainTimes[currentPointer],
    "KEY_RAINPERHOUR": rainPerHour[currentPointer] + "",
    "KEY_RAINUNIT": null,
    "KEY_NOW": null,
    "KEY_REFRESH": null,
    "KEY_TEMP": temps[currentPointer],
    "KEY_DATE": dates[currentPointer],
    "KEY_LONGTERM": null,
    "KEY_24H": ampms[currentPointer]
  };

  currentPointer++;

  if(currentPointer <= currentAmount) {

    // Send to Pebble
    Pebble.sendAppMessage(dictionary,
      function(e) {
        console.log("Rain info sent to Pebble successfully!");
        sendNextData();
      },
      function(e) {
        console.log("Error sending next info to Pebble!");
      }
    );

  } else {
    busy = false;

    if (getCityAfterEvrything) {
      getCity(currentPos, false);
    }
  }
}

// Listen for when the app is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");

    // Notify the watchapp that it is now safe to send messages
    var dictionary = {
      "KEY_READY": "true",
      "KEY_LOCATION": null,
      "KEY_RAINDROPS": null,
      "KEY_RAINTIMES": null,
      "KEY_RAINPERHOUR": null,
      "KEY_RAINUNIT": null,
      "KEY_NOW": null,
      "KEY_REFRESH": null,
      "KEY_TEMP": null,
      "KEY_DATE": null,
      "KEY_LONGTERM": null,
      "KEY_24H": null
    };

    // Send to Pebble
    Pebble.sendAppMessage(dictionary,
      function(e) {
        console.log("Readyness sent to Pebble successfully!");
      },
      function(e) {
        console.log("Error sending ready info to Pebble!");
        console.log(JSON.stringify(e));
      }
    );
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received: " +  JSON.stringify(e.payload));

    if(e.payload['KEY_LONGTERM'] && e.payload['KEY_LONGTERM'] == 1) {
      longTerm = true;
    }else{
      longTerm = false;
    }

    if(e.payload['KEY_24H'] && e.payload['KEY_24H'] == 1) {
      hours24 = true;
    }else{
      hours24 = false;
    }

    if (!busy) {
      getLocation();
    }
  }                     
);

// Listen for configuration window
Pebble.addEventListener('showConfiguration', function(e) {
  // Show config page
  Pebble.openURL('http://vaw.be/rain/config-1-9.html');
});

// Listen for config close
Pebble.addEventListener('webviewclosed', function(e) {
  var config = JSON.parse(decodeURIComponent(e.response));

  console.log('Configuration page returned: ' + JSON.stringify(config));

  localStorage.setItem(0, config.api);
  localStorage.setItem(1, config.apiKey);
  localStorage.setItem(2, config.gps);
  localStorage.setItem(3, config.lat);
  localStorage.setItem(4, config.lng);
  localStorage.setItem(5, config.units);
  localStorage.setItem(6, config.refresh);
  localStorage.setItem(7, config.volume);

  getLocation();

});