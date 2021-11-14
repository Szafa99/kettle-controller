// const evt="setAimedTemp37.00";
// console.log(evt.substr("setAimedTemp".length,evt.length) + " ℃");
// console.log(evt.includes("AimedTemp"))


let str="setMachineTime30:00";
const txt = str.substr("setMachineTime".length,str.length);
let min=txt.substr(0,txt.indexOf(':'));
let sec=txt.substr(txt.indexOf(':')+1,txt.length)
console.log(sec);




