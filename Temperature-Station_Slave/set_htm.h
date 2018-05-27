const String devSetPage = R"a(<!DOCTYPE html>
<html>
  <head>
    <meta http-equiv="cache-control" content="no-cache" />
    <title>Temperature Station</title>
    <style>table,th,td {border: 1px solid black;
      text-align: center}
    </style>
  </head>
  <body onload="loadSettings()">
    <a href="/"><button>Strona glowna</button></a>
    <form method="POST" id="general" action="/settings">
      <table align="center">
        <caption>General settings</caption>
        <input type="hidden" id="action" name="action">
        <input type="hidden" id="_outDHCP" value="0" name="outDHCP">
        <tr>
          <td>Name:</td>
          <td><input type="text" id="selfName" size="20" name="selfName">
          </td>
        </tr>
        <tr>
          <td>SSID:</td>
          <td>
            <input type="text" id="outSSID" size="20" name="outSSID">
          </td>
        </tr>
        <tr>
          <td>Passphrase:</td>
          <td>
            <input type="password" id="outPASS" size="20" name="outPASS">
          </td>
        </tr>
        <tr>
          <td>AP SSID:</td>
          <td>
            <input type="text" id="selfSSID" size="20" name="selfSSID">
          </td>
        </tr>
        <tr>
          <td>AP Passphrase:</td>
          <td>
            <input type="text" id="selfPASS" size="20" name="selfPASS">
          </td>
        </tr>
        <tr>
          <td>IP Address:</td>
          <td>
            <input type="text" id="outIP" size="20" name="outIP">
          </td>
        </tr>
        <tr>
          <td>Gateway:</td>
          <td>
            <input type="text" id="outGW" size="20" name="outGW">
          </td>
        </tr>
        <tr>
          <td>Broadcast:</td>
          <td>
            <input type="text" id="outMASK" size="20" name="outMASK">
          </td>
        </tr>
        <tr>
          <td>Use DHCP:</td>
          <td>
            <input type="checkbox" id="outDHCP" value="1" name="outDHCP">Enable
          </td>
        </tr>
        <tr>
          <td>Login:</td>
          <td>
            <input type="text" id="login" size="20" name="login">
          </td>
        </tr>
        <tr>
          <td>New login:</td>
          <td>
            <input type="text" id="n_login" size="20" name="n_login">
          </td>
        </tr>
        <tr>
          <td>Pasword:</td>
          <td>
            <input type="password" id="password" size="20" name="password">
          </td>
        </tr>
        <tr>
          <td>New Pasword:</td>
          <td>
            <input type="password" id="new_password" size="20" name="new_password">
          </td>
        </tr>
        <tr>
          <td>Repeat Pasword:</td>
          <td>
            <input type="password" id="r_password" size="20" name="r_password">
          </td>
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
              document.getElementById("outSSID").value = resp.outSSID;
              document.getElementById("outIP").value = resp.outIP;
              document.getElementById("outGW").value = resp.outGW;
              document.getElementById("outMASK").value = resp.outMASK;
              document.getElementById("outDHCP").checked = resp.outDHCP;
              document.getElementById("login").checked = resp.login;
          };
          return;}
      function reset() {
          var xhttp = new XMLHttpRequest();
          xhttp.open("POST", "/settings", true);
          xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
          xhttp.send("action=reset");
          return;
      }
      
      function uploadGen(toDo = undefined) {
          var tmp = document.getElementById("outIP").value;
          var submitOK = true;
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
          if (document.getElementById("outDHCP").checked)
              document.getElementById("_outDHCP").disabled = true;
          else document.getElementById("outDHCP").disabled = true;
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
          if(submitOK == true){
          var xhr = new XMLHttpRequest();
          xhr.open("POST", "/settings", true);
          xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
          xhr.send(postData.join("&"));
        } else alert("Check fields");
          document.getElementById("_outDHCP").disabled = false;
          document.getElementById("outDHCP").disabled = false;
          return;
      }
      function saveGen() {uploadGen("save");return;}
      function testGen() {uploadGen("test");return;}
    </script>
  </body>
</html>
)a";
