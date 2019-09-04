const String devSetPage = R"a(
<!DOCTYPE html><html><head>
<title>Temperature Station</title>
<meta http-equiv="cache-control" content="no-cache" />
<style>table,th,td {border: 1px solid black;text-align: center}</style>
</head><body onload="loadSettings()">
<a href="/"><button>Strona glowna</button></a>
<form method="POST" id="general" a="/settings">
<table align="center">
<caption>General settings</caption>
<tr><td>Name:</td>
<td><input type="text" id="SN" size="20" name="SN"></td></tr>
<tr><td>AP SSID:</td>
<td><input type="text" id="SS" size="20" name="SS"></td></tr>
<tr><td>Update AP:</td>
<td><input type="checkbox" id="ch_ap" name="ch_ap"></td></tr>
<tr><td>AP WPA2 Key:</td>
<td><input type="text" id="SPA" size="20" name="SPA"></td></tr>
<tr><td>Login:</td>
<td><input type="text" id="SL" size="20" name="SL"></td></tr>
<tr><td>Pasword:</td>
<td><input type="password" id="SPL" size="20" name="SPL"></td></tr>
<tr><td>New login:</td>
<td><input type="text" id="NL" size="20" name="NL"></td></tr>
<tr><td>New pasword:</td>
<td><input type="password" id="NP" size="20" name="NP"></td></tr>
<tr><td>Repeat Pasword:</td>
<td><input type="password" id="RNP" size="20" name="RNP"></td></tr>
<tr><td>Network SSID:</td>
<td><input type="text" id="OS" size="20" name="OS"></td></tr>
<tr><td>Update WiFi:</td>
<td><input type="checkbox" id="ch_wi" name="ch_wi"></td></tr>
<tr><td>Network Passphrase:</td>
<td><input type="text" id="OP" size="20" name="OP"></td></tr>
<tr><td>IP Address:</td>
<td><input type="text" id="OI" size="20" name="OI"></td></tr>
<tr><td>Gateway:</td>
<td><input type="text" id="OG" size="20" name="OG"></td></tr>
<tr><td>Broadcast:</td>
<td><input type="text" id="OM" size="20" name="OM"></td></tr>
<tr><td>Use DHCP:</td>
<td><input type="checkbox" id="OD" name="OD">Enable</td></tr>
</table><input type="button" align="center" onclick="uploadGen()" value="Save"></form>
<input type="button" align="center" onclick="reset()" value="Defaults">
<br><script>
function loadSettings(){var t=new XMLHttpRequest;t.open("POST","/settings",!0),t.setRequestHeader("Content-type","application/x-www-form-urlencoded"),t.send("a=l"),t.onreadystatechange=function(){console.log(t.responseText);var e=JSON.parse(t.responseText),a=document.forms.general;document.forms.time;a.SN.placeholder=e.SN,a.SS.placeholder=e.SS,a.OS.placeholder=e.OS,a.OI.placeholder=e.OI,a.OG.placeholder=e.OG,a.OM.placeholder=e.OM,a.SL.value=e.SL,a.OD.checked=e.OD,a.ch_ap.addEventListener("change",function(){ch_box(0)}),a.ch_wi.addEventListener("change",function(){ch_box(1)}),ch_box(0),ch_box(1)}}function ch_box(e){var a=document.forms.general;0==e?(a.SS.disabled=!a.ch_ap.checked,a.SPA.disabled=!a.ch_ap.checked):1==e&&(a.OS.disabled=!a.ch_wi.checked,a.OP.disabled=!a.ch_wi.checked)}function uploadGen(){for(var e=!0,a=document.forms.general,t=[],d=0;d<a.length;d++){("text"!=(s=a[d]).type&&"password"!=s.type||""==s.value)&&(s.disabled=!0)}if(1==a.ch_ap.checked&&(""==a.SS.value?(e=!1,alert("Check AP settings")):(t.push("SS="+a.SS.value),t.push("SPA="+a.SPA.value),a.SPA.disabled=!0,a.SS.disabled=!0)),1==a.ch_wi.checked&&(""==a.OS.value?(e=!1,alert("Check WiFi settings")):(t.push("OS="+a.OS.value),t.push("OP="+a.OP.value),a.OS.disabled=!0,a.OP.disabled=!0)),a.OI.disabled||validIP(a.OI.value)||(e=!1,alert("Incorrect IP address!")),a.OG.disabled||validIP(a.OG.value)||(e=!1,alert("Incorrect gateway address!")),a.OM.disabled||validIP(a.OM.value)||(e=!1,alert("Incorrect subnet mask!")),a.NP.disabled||a.RNP.disabled||(a.NP.value==a.RNP.value?""==a.NP.value&&(a.NP.disabled=!0):(e=!(a.RNP.disabled=!0),alert("Passwords don't match")),a.RNP.disabled=!0),null==a.NL.value&&(a.NL.disabled=!0),a.SL.disabled=!1,a.SPL.disabled=!1,e){t.push("a=s"),1==a.OD.checked?t.push("OD=1"):t.push("OD=0");for(d=0;d<a.length;d++){var s;(s=a[d]).disabled||t.push(s.name+"="+s.value)}var l=new XMLHttpRequest;l.onreadystatechange=function(){4===l.readyState&&(200!==l.status&&401!==l.status||alert(l.responseText))},l.open("POST","/settings",!0),l.setRequestHeader("Content-type","application/x-www-form-urlencoded"),l.send(t.join("&")),console.log(t.join("&"))}for(d=0;d<a.length;d++)a[d].disabled=!1}function validIP(e){return!!/^[0-9.]+$/.test(e)&&(3==e.replace(/[^.]/g,"").length&&!(15<e.length))}function reset(){var e=new XMLHttpRequest;e.open("POST","/settings",!0),e.setRequestHeader("Content-type","application/x-www-form-urlencoded"),e.send("a=r")}
</script></body></html>
)a";
