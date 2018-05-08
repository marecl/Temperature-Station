String devSetPage = R"a(<!DOCTYPE html>
<html><head>
  <meta http-equiv="cache-control" content="no-cache" />
  <title> Temperature Station</title>
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
  <a href="/"><button>Strona glowna</button></a>
  <form method="POST" id="general" action="/settings">
    <table align="center">
    <caption>General settings</caption>
    <input type="hidden" id="action" name="action">
    <input type="hidden" id="_selfAP" value="0" name="selfAP">
    <input type="hidden" id="_outDHCP" value="0" name="outDHCP">
    <tr><td>Name:</td>
    <td><input type="text" id="selfName" size="20" name="selfName"></td>
    </tr><tr>
    <td>SSID:</td>
    <td><input type="text" id="selfSSID" size="20" name="selfSSID"></td>
    </tr><tr>
    <td>Self AP:</td>
    <td><input type="checkbox" id="selfAP" value="1" name="selfAP">Enable</td>
    </tr><tr>
    <td>WPA2 Key:</td>
    <td><input type="text" id="selfWPA2" size="20" name="selfWPA2"></td>
    </tr><tr>
    <td>Login:</td>
    <td><input type="text" id="selfLOGIN" size="20" name="selfLOGIN"></td>
    </tr><tr>
    <td>Pasword:</td>
    <td><input type="password" id="selfPASS" size="20" name="selfPASS"></td>
    </tr><tr>
    <td>Repeat Pasword:</td>
    <td><input type="password" id="repeatPASS" size="20" name="repeatPASS"></td>
    </tr><tr>
    <td>Network SSID:</td>
    <td><input type="text" id="outSSID" size="20" name="outSSID"></td>
    </tr><tr>
    <td>Network Passphrase:</td>
    <td><input type="text" id="outPASS" size="20" name="outPASS"></td>
    </tr><tr>
    <td>IP Address:</td>
    <td><input type="text" id="outIP" size="20" name="outIP"></td>
    </tr><tr>
    <td>Gateway:</td>
    <td><input type="text" id="outGW" size="20" name="outGW"></td>
    </tr><tr>
    <td>Broadcast:</td>
    <td><input type="text" id="outMASK" size="20" name="outMASK"></td>
    </tr><tr>
    <td>Use DHCP:</td>
    <td><input type="checkbox" id="outDHCP" value="1" name="outDHCP">Enable</td>
    </tr>
    </table>
    <input type="button" align="center" onclick="saveGen()" value="Save">
    <input type="button" align="center" onclick="testGen()" value="Test">
  </form>
  <br>  
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
    document.getElementById("selfLOGIN").value = resp.selfLOGIN;
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
  </script>
</body></html>)a";
