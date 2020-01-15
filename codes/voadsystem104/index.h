const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>VOAD System Testing Page</title>
    <style>
        body {
            text-align: center;
        }
    </style>
</head>
<body>
    <h1>VOAD System Testing Page</h1>
    <h3>Latitude: <span id="latitude"></span></h3>
    <h3>Longitude: <span id="longitude"></span></h3>
    <h3>Speed: <span id="speed"></span></h3>

    <script>
        setInterval(function() {
            getLatitudeData();
            getLongitudeData();
        }, 1000);

        setInterval(function() {
            getSpeedData();
        }, 100);

        function getLatitudeData() {
            let xhttp =new XMLHttpRequest();
            xhttp.open("GET", "readLatitude", true);

            xhttp.onload = function() {
                if(this.status == 200) {
                    document.querySelector('#latitude').innerHTML =
                    this.responseText;
                }
            };
            xhttp.send();
        }

        function getLongitudeData() {
            let xhttp =new XMLHttpRequest();
            xhttp.open("GET", "readLongitude", true);

            xhttp.onload = function() {
                if(this.status == 200) {
                    document.querySelector('#longitude').innerHTML =
                    this.responseText;
                }
            };
            xhttp.send();
        }

        function getSpeedData() {
            let xhttp =new XMLHttpRequest();
            xhttp.open("GET", "readSpeed", true);

            xhttp.onload = function() {
                if(this.status == 200) {
                    document.querySelector('#speed').innerHTML =
                    this.responseText;
                }
            };
            xhttp.send();
        }
    </script>
</body>
</html>
)=====";
