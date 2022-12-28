var name = "Anon";
var nim = 0;

var queryString = window.location.search.split('&');

document.getElementById("logout").onclick = reset() ;

var name = `${((queryString[0]).split('='))[1].substring(0,1).toUpperCase()}${((queryString[0]).split('='))[1].substring(1)}`.replaceAll('+', ' ');
var nim = ((queryString[1]).split('='))[1] ;

document.getElementById('greetings').innerHTML = `Greetings ${name}!` ;
document.getElementById('nimgreet').innerHTML = `NIM: ${nim}` ;

function reset() {
    //Kosongin nama dan nim
    var name = "Anon";
    var nim = 0 ;
}

function setClock() {
    var dayName = ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'] ;
    var monthName = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sept', 'Oct', 'Nov', 'Dec'] ;
    var date = new Date() ;
    var sec = date.getSeconds();
    var min = date.getMinutes() ;
    var hour = date.getHours() ;
    var amPm = '' ;
    var greeting = '' ;
    if (hour == 24) {
        hour = 0 ;
    }
    if (hour < 12) {
        amPm = "AM" ;
    }else {
        amPm = "PM" ;
        if (hour > 12) {
            hour -= 12 ;
        }
    }
    if (sec < 10) {
        sec = '0' + sec ;
    }
    if (min < 10) {
        min = '0' + min ;
    }
    if (hour < 10) {
        hour = '0' + hour ;
    }
    document.getElementById('date').innerHTML = `${dayName[date.getDay()]}, ${monthName[date.getMonth()]} ${date.getFullYear()}` ;
    document.getElementById('serv-time').innerHTML = `Server Time: ${hour}:${min}:${sec} ${amPm}`;
    
    if (hour >= 5 && hour < 12 && amPm == "AM") {
        greeting = "Morning";
    }else if (hour >= 5 && hour <= 12 && amPm == "PM") {
        greeting = "Afternoon";
    }else if (hour > 5 && hour <= 9 && amPm == "PM") {
        greeting = "Evening";
    }else {
        greeting = "Night";
    }
    
    document.getElementById('dayGreet').innerHTML = `Good ${greeting}!` ;
}
setInterval(setClock, 1000) ;