static const String html_settings = R"eof(<!DOCTYPE html>
<html>
 <title>Temperature Station</title>
  <head>
    <meta http-equiv="cache-control" content="no-cache" />
    <style>
      table,
      th,
      td {
      border: 1px solid black;
      text-align: center
      }
    </style>
  </head>
  <body onload="loadSettings()">
    <a href="/">
    <button>Strona glowna</button>
    </a>
    <form method="POST" id="general" action="/settings">
      <table align="center">
        <caption>General settings</caption>
        <input type="hidden" id="action" name="action">
        <input type="hidden" id="_selfAP" value="0" name="selfAP">
        <input type="hidden" id="_outDHCP" value="0" name="outDHCP">
        <tr>
          <td>Name:</td>
          <td><input type="text" id="selfName" size="20" name="selfName"></td>
        </tr>
        <tr>
          <td>SSID:</td>
          <td><input type="text" id="selfSSID" size="20" name="selfSSID"></td>
        </tr>
        <tr>
          <td>Self AP:</td>
          <td><input type="checkbox" id="selfAP" value="1" name="selfAP">Enable</td>
        </tr>
        <tr>
          <td>WPA2 Key:</td>
          <td><input type="text" id="selfWPA2" size="20" name="selfWPA2"></td>
        </tr>
        <tr>
          <td>Login:</td>
          <td><input type="text" id="selfLOGIN" size="20" name="selfLOGIN"></td>
        </tr>
        <tr>
          <td>Pasword:</td>
          <td><input type="password" id="selfPASS" size="20" name="selfPASS"></td>
        </tr>
        <tr>
          <td>New login:</td>
          <td><input type="text" id="newLOGIN" size="20" name="newLOGIN"></td>
        </tr>
        <tr>
          <td>New pasword:</td>
          <td><input type="password" id="newPASS" size="20" name="newPASS"></td>
        </tr>
        <tr>
          <td>Repeat Pasword:</td>
          <td><input type="password" id="repeatPASS" size="20" name="repeatPASS"></td>
        </tr>
        <tr>
          <td>Network SSID:</td>
          <td><input type="text" id="outSSID" size="20" name="outSSID"></td>
        </tr>
        <tr>
          <td>Network Passphrase:</td>
          <td><input type="text" id="outPASS" size="20" name="outPASS"></td>
        </tr>
        <tr>
          <td>IP Address:</td>
          <td><input type="text" id="outIP" size="20" name="outIP"></td>
        </tr>
        <tr>
          <td>Gateway:</td>
          <td><input type="text" id="outGW" size="20" name="outGW"></td>
        </tr>
        <tr>
          <td>Broadcast:</td>
          <td><input type="text" id="outMASK" size="20" name="outMASK"></td>
        </tr>
        <tr>
          <td>Use DHCP:</td>
          <td><input type="checkbox" id="outDHCP" value="1" name="outDHCP">Enable</td>
        </tr>
      </table>
      <input type="button" align="center" onclick="saveGen()" value="Save">
      <input type="button" align="center" onclick="testGen()" value="Test">
    </form>
    <br>
    <form method="POST" id="time" action="/time">
      <table align="center">
        <caption>Time related settings</caption>
        <input type="hidden" id="action" name="action">
        <input type="hidden" id="epoch" name="epoch">
        <input type="hidden" id="_useNTP" value="0" name="useNTP">
        <tr>
          <td>Date and time:</td>
          <td><input type="datetime-local" id="datetime" name="datetime" min="2000-01-01T00:00"></td>
        </tr>
        <tr>
          <td>Timezone:</td>
          <td><input type="text" id="timezone" size="20" name="timezone"></td>
        </tr>
        <tr>
          <td>Use NTP Server:</td>
          <td><input type="checkbox" id="useNTP" value="1" name="useNTP"></td>
        </tr>
        <tr>
          <td>NTP Server:</td>
          <td><input type="text" id="ntpServer" name="ntpServer"></td>
        </tr>
        <input type="button" align="center" onclick="saveTime()" value="Save">
        <input type="button" align="center" onclick="getUserTime()" value="Get user time">
        <input type="button" align="center" onclick="testTime()" value="Test">
      </table>
    </form>
    <input type="button" align="center" onclick="reset()" value="Defaults">
    <br>
    <script>
      function loadSettings() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("POST", "/settings", true);
      xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
      xhttp.send("action=load");
      xhttp.onreadystatechange = function() {
      console.log(xhttp.responseText);
      var resp = JSON.parse(xhttp.responseText);
      document.getElementById("selfName").value = resp.selfName;
      document.getElementById("selfSSID").value = resp.selfSSID;
      document.getElementById("selfAP").checked = resp.selfAP;
      document.getElementById("selfLOGIN").value = resp.login;
      document.getElementById("outSSID").value = resp.outSSID;
      document.getElementById("outIP").value = resp.outIP;
      document.getElementById("outGW").value = resp.outGW;
      document.getElementById("outMASK").value = resp.outMASK;
      document.getElementById("outDHCP").checked = resp.outDHCP;
      document.getElementById("useNTP").checked = resp.useNTP;
      document.getElementById("ntpServer").value = resp.ntpServer;
      document.getElementById("timezone").value = resp.timezone;
      document.getElementById("datetime").value = resp.dateTime;
      };
      return;
      }
      
      function reset() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("POST", "/settings", true);
      xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
      xhttp.send("action=reset");
      return;
      }
      
      function uploadGen(toDo = undefined) {
      var tmp = document.getElementById("outIP").value;
      if (!(/^[0-9.]+$/.test(tmp)) || (tmp.replace(/[^.]/g, "").length != 3) || tmp.length > 15) {
      submitOK = false;
      }
    tmp = document.getElementById("outGW").value;
      if (!(/^[0-9.]+$/.test(tmp)) || (tmp.replace(/[^.]/g, "").length != 3) || tmp.length > 15) {
      submitOK = false;
      }
    tmp = document.getElementById("outMASK").value;
      if (!(/^[0-9.]+$/.test(tmp)) || (tmp.replace(/[^.]/g, "").length != 3) || tmp.length > 15) {
      submitOK = false;
      }
    tmp = document.getElementById("timezone").value;
    if(tmp < -12 || tmp > 12){
    submitOK = false;
    }
    tmp = document.getElementById("newPASS").value;
    var tmp2 = document.getElementById("repeatPASS").value;
    if(tmp == tmp2 && tmp != undefined){
    document.getElementById("newPASS").disabled = true;
    document.getElementById("repeatPASS").disabled = true;
    } else submitOK = false;

    tmp = document.getElementById("newLOGIN").value;
    if(tmp == undefined)
    document.getElementById("newLOGIN").disabled = true;

    
    if (document.getElementById("newLOGIN").value == undefined)
      document.getElementById("newLOGIN").disabled = true;
    if (document.getElementById("newPASS").value == undefined)
      document.getElementById("newPASS").disabled = true;
    if (document.getElementById("repeatPASS").value == undefined)
      document.getElementById("repeatPASS").disabled = true;
      
      if (document.getElementById("outDHCP").checked)
      document.getElementById("_outDHCP").disabled = true;
      else document.getElementById("outDHCP").disabled = true;
      
      if (document.getElementById("selfAP").checked)
      document.getElementById("_selfAP").disabled = true;
      else document.getElementById("selfAP").disabled = true;
      
      if (toDo != undefined) {
      document.getElementById("action").disabled = false;
      document.getElementById("action").value = toDo;
      }
      document.forms.general.action.value = toDo;
      var f = document.forms.general;
      console.log(f.action.value);
      var postData = [];
      for (var i = 0; i < f.elements.length; i++) {
      if (f.elements[i].value != "" && f.elements[i].name != "" && f.elements[i].disabled == false)
      postData.push(f.elements[i].name + "=" + f.elements[i].value);
      }
      var xhr = new XMLHttpRequest();
      xhr.open("POST", "/settings", true);
      xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
      xhr.send(postData.join("&"));
      
      document.getElementById("_outDHCP").disabled = false;
      document.getElementById("outDHCP").disabled = false;
      document.getElementById("_selfAP").disabled = false;
      document.getElementById("selfAP").disabled = false;
      
      return;
      }
      
      function saveGen() {
      uploadGen("save");
      return;
      }
      
      function testGen() {
      uploadGen("test");
      return;
      }
      
      function uploadTime(toDo = undefined) {
      if (document.getElementById("useNTP").checked)
      document.getElementById("_useNTP").disabled = true;
      else document.getElementById("useNTP").disabled = true;
      
      if (toDo != undefined) {
      document.getElementById("action").disabled = false;
      document.getElementById("action").value = toDo;
      }
      
      document.forms.time.action.value = toDo;
      var g = document.forms.time;
      console.log(g.action.value);
      var postData = [];

      var epo = new Date(document.getElementById("datetime").value).getTime();
      document.getElementById("datetime").disabled = true;
      document.getElementById("epoch").value = (epo / 1000);
      
      for (var i = 0; i < g.elements.length; i++) {
      if (g.elements[i].value != "" && g.elements[i].name != "" && g.elements[i].disabled == false) {
      postData.push(g.elements[i].name + "=" + g.elements[i].value);
      console.log(g.elements[i].name + "=" + g.elements[i].value);
      }
      }
      document.getElementById("datetime").disabled = false;
      var xhr = new XMLHttpRequest();
      xhr.open("POST", "/time", true);
      xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
      xhr.send(postData.join("&"));
      document.getElementById("_useNTP").disabled = false;
      document.getElementById("useNTP").disabled = false;
      return;
      }
      function saveTime() {
      uploadTime("save");
      return;
      }
      
      function testTime() {
      var ep = new Date(document.getElementById("datetime").value).getTime();
      document.getElementById("datetime").disabled = true;
      document.getElementById("epoch").value = (ep / 1000);
      uploadTime("test");
      document.getElementById("datetime").disabled = false;
      return;
      }
    </script>
  </body>
</html>)eof";
