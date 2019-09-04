#include<pgmspace.h>
char settings_htm[] PROGMEM = R"abcd(
<!DOCTYPE html><html><head><title>Temperature Station</title><meta http-equiv="cache-control" content="no-cache" /><style>table,
th,
td{
border: 1px solid black;
text-align: center}
</style></head><body onload="loadSettings()"><a href="/"><button>Strona glowna</button></a><form method="POST" id="general"><table align="center"><caption>General settings</caption><tr><td>Name:</td><td><input type="text" id="SN" size="20" name="SN"></td></tr><tr><td>AP SSID:</td><td><input type="text" id="SS" size="20" name="SS"></td></tr><tr><td>Update AP:</td><td><input type="checkbox" id="ch_ap" name="ch_ap"></td></tr><tr><td>AP WPA2 Key:</td><td><input type="text" id="SPA" size="20" name="SPA"></td></tr><tr><td>Login:</td><td><input type="text" id="SL" size="20" name="SL"></td></tr><tr><td>Pasword:</td><td><input type="password" id="SPL" size="20" name="SPL"></td></tr><tr><td>New login:</td><td><input type="text" id="NL" size="20" name="NL"></td></tr><tr><td>New pasword:</td><td><input type="password" id="NP" size="20" name="NP"></td></tr><tr><td>Repeat Pasword:</td><td><input type="password" id="RNP" size="20" name="RNP"></td></tr><tr><td>Network SSID:</td><td><input type="text" id="OS" size="20" name="OS"></td></tr><tr><td>Update WiFi:</td><td><input type="checkbox" id="ch_wi" name="ch_wi"></td></tr><tr><td>Network Passphrase:</td><td><input type="text" id="OP" size="20" name="OP"></td></tr><tr><td>IP Address:</td><td><input type="text" id="OI" size="20" name="OI"></td></tr><tr><td>Gateway:</td><td><input type="text" id="OG" size="20" name="OG"></td></tr><tr><td>Broadcast:</td><td><input type="text" id="OM" size="20" name="OM"></td></tr><tr><td>Use DHCP:</td><td><input type="checkbox" id="OD" name="OD">Enable</td></tr></table><input type="button" onclick="uploadGen()" value="Save"></form>
<input type="button" onclick="reset()" value="Factory reset"><br><script>function loadSettings(){
var xhr=new XMLHttpRequest();
xhr.open("POST","/settings",true);
xhr.setRequestHeader("Content-type","application/x-www-form-urlencoded");
xhr.send("a=l");
xhr.onreadystatechange=function(){
if(xhr.status===200&&xhr.readyState===4){
console.log(xhr.responseText);
var resp=JSON.parse(xhr.responseText);
var z=document.forms.general;
z.SN.placeholder=resp.SN;
z.SS.placeholder=resp.SS;
z.OS.placeholder=resp.OS;
z.OI.placeholder=resp.OI;
z.OG.placeholder=resp.OG;
z.OM.placeholder=resp.OM;
z.SL.value=resp.SL;
z.OD.checked=resp.OD;
z.ch_ap.addEventListener("change",function(){
var z=document.forms.general;
z.SS.disabled=z.ch_ap.checked?false:true;
z.SPA.disabled=z.ch_ap.checked?false:true;});
z.ch_wi.addEventListener("change",function(){
var z=document.forms.general;
z.OS.disabled=z.ch_wi.checked?false:true;
z.OP.disabled=z.ch_wi.checked?false:true;});
z.SS.disabled=true;
z.SPA.disabled=true;
z.OS.disabled=true;
z.OP.disabled=true;}};
return;}
function uploadGen(){
var submitOK=true;
var a=document.forms.general;
var postData=["a=s"];
for(var i=0;i<a.length;i++){
var b=a[i];
if((b.type!="text"&&b.type!="password")||b.value==""){
b.disabled=true;}}
a.SL.disabled=true;
a.SPL.disabled=true;
if(a.ch_ap.checked==true){
if(a.SS.value==""){
submitOK=false;
alert("Check AP settings");}else{
postData.push("SS="+a.SS.value);
postData.push("SPA="+a.SPA.value);
a.SPA.disabled=true;
a.SS.disabled=true;}}
if(a.ch_wi.checked==true){
if(a.OS.value==""){
submitOK=false;
alert("Check WiFi settings");}else{
postData.push("OS="+a.OS.value);
postData.push("OP="+a.OP.value);
a.OS.disabled=true;
a.OP.disabled=true;}}
if(!a.OI.disabled&&!validIP(a.OI.value)){
submitOK=false;
alert("Incorrect IP address!");}
if(!a.OG.disabled&&!validIP(a.OG.value)){
submitOK=false;
alert("Incorrect gateway address!");}
if(!a.OM.disabled&&!validIP(a.OM.value)){
submitOK=false;
alert("Incorrect subnet mask!");}
if(!a.NP.disabled&&!a.RNP.disabled){
if(a.NP.value==a.RNP.value){
if(a.NP.value==""){
a.NP.disabled=true;}}else{
a.RNP.disabled=true;
submitOK=false;
alert("Passwords don't match");}
a.RNP.disabled=true;}
if(a.NL.value==""){
a.NL.disabled=true;}
if(submitOK){
if(a.OD.checked==true){
postData.push("OD=1");}else{
postData.push("OD=0");}
for(var i=0;i<a.length;i++){
var b=a[i];
if(!b.disabled){
postData.push(b.name+"="+b.value);}}
upload(postData,"/settings");}
for(var i=0;i<a.length;i++){
a[i].disabled=false;}
return;}
function validIP(_i){
if(!/^[0-9.]+$/.test(_i)){
return false;}
if(_i.replace(/[^.]/g,"").length!=3){
return false;}
if(_i.length>15){
return false;}
return true;}
function reset(){
upload(["a=r"],"/settings");
return;}
function upload(what,where){
var a=document.forms.general;
what.push("SL="+a.SL.value);
what.push("SPL="+a.SPL.value);
console.log(what);
var xhr=new XMLHttpRequest();
xhr.onreadystatechange=function(){
if(xhr.readyState===4){
if(xhr.status===200||xhr.status===401){
alert(xhr.responseText);}}}
xhr.open("POST",where,true);
xhr.setRequestHeader("Content-type","application/x-www-form-urlencoded");
xhr.send(what.join("&"));}
</script></body></html>
)abcd";
