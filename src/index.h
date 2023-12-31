const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
  body {
    margin: 0;
    padding: 0;
  }

  .container {
    display: flex;
    justify-content: space-around;
    align-items: center;
    height: 100vh;
  }

  .slider {
    background: white;
    border: 1px solid black;
  }

  .inner {
    display: flex;
    justify-content: center;
    width: 50vw;
  }

  #speed {
    width: 200px;
    max-height: 700px;
    height: 80vh;
    background-color: red;
  }

  #speed .indicator {
    width: 200px;
    height: ?SPEED_PERCENTAGE?;
    background-color: white;
  }

  #steering {
    max-width: 700px;
    width: 40vw;
    height: 200px;
  }

  #steering .indicator {
    height: 200px;
    width: ?STEERING_PERCENTAGE?;
    background-color: red;
  }
</style>

<body>
  <div class="container">
    <div class="inner">
      <div class="slider" id="speed">
        <div class="indicator"></div>
      </div>

    </div>
    <div class="inner">
      <div class="slider" id="steering">
        <div class="indicator"></div>
      </div>
    </div>
  </div>
</body>

</html>

<script>
  let speed = 0;
  let steering = 0;

  set_handlers("speed");
  set_handlers("steering");

  function set_handlers(name) {
    console.log(name);
    const el = document.getElementById(name);
    el.ontouchstart = handler;
    el.ontouchmove = handler;
  }

  function handler(ev) {
    ev.preventDefault();
    const touch = ev.touches[0];

    let indicator;
    let parent;
    if (touch.target.id === "speed" || touch.target.id === "steering") {
      indicator = ev.target.getElementsByClassName("indicator")[0];
      parent = ev.target;
    } else {
      indicator = ev.target;
      parent = ev.target.parentElement;
    }

    const boundingBox = parent.getBoundingClientRect();

    if (checkBoundary(boundingBox, touch)) {
      xPercent = (touch.clientX - boundingBox.x) / boundingBox.width * 100;
      yPercent = (touch.clientY - boundingBox.y) / boundingBox.height * 100;

      if (parent.id === "speed") {
        indicator.style.height = `${yPercent}%`;
        speed = limitNumberWithinRange((100 - yPercent) / 100 * (255 - -255) + -255);
      } else if (parent.id === "steering") {
        indicator.style.width = `${xPercent}%`;
        steering = limitNumberWithinRange(xPercent / 100 * (255 - -255) + -255);
      }
    }
  }

  function limitNumberWithinRange(num) {
    const MIN = -255;
    const MAX = 255;
    const parsed = parseInt(num)
    return Math.min(Math.max(parsed, MIN), MAX)
  }


  function checkBoundary(boundingBox, touch) {
    if (boundingBox.top <= touch.clientY &&
      boundingBox.bottom >= touch.clientY &&
      boundingBox.left <= touch.clientX &&
      boundingBox.right >= touch.clientX) {
      return true;
    }

    return false;
  }

  const gateway = `ws://${window.location.hostname}/ws`;
  let websocket;

  console.log('Establishing connection...');
  initWebSocket();

  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    // websocket.onmessage = onMessage;

    setInterval(updateRobot, 200);
  }

  function onOpen(event) {
    console.log('Connection opened');
  }

  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }

  // function onMessage(event) {
    // const data = event.data.split(',');
    // speed = data[0];
    // steering = data[1];
    // document.getElementById("speed").getElementsByClassName("indicator")[0].style.height = `${speed}%`;
    // document.getElementById("steering").getElementsByClassName("indicator")[0].style.width = `${steering}%`;
  // }

  function updateRobot() {
    if (websocket.readyState === websocket.OPEN) {
      websocket.send(speed + ',' + steering);
    }
  }
</script>
</body>

</html>
)=====";
