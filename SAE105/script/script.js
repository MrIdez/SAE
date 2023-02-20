const header = document.getElementById("header") 
const menuTel = document.getElementById("menuTel")

let headerExpended = false

menuTel.addEventListener("click", () => {
    if (headerExpended) {
        header.className = ""
        headerExpended = !headerExpended
    }
    else {
        header.className = "expend"
        headerExpended = !headerExpended
    }

})