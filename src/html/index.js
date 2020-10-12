toastr.options = {
  "progressBar": true,
  "closeButton": true,
  "debug": false,
  "newestOnTop": false,
  "positionClass": "toast-bottom-right",
  "preventDuplicates": true,
  "onclick": null,
  "showDuration": "200",
  "hideDuration": "1000",
  "timeOut": "5000",
  "extendedTimeOut": "1000",
  "showEasing": "swing",
  "hideEasing": "linear",
  "showMethod": "fadeIn",
  "hideMethod": "fadeOut"
}

function definitionCopied(){
    toastr["success"]("Definition copied to clipboard.", "Clipboard")
}

function about(){
    $.tinyModal({
        title: 'About',
        html: '#about'
    });
}


function simulateMouseClick(targetNode) {
    function triggerMouseEvent(targetNode, eventType) {
        var clickEvent = document.createEvent('MouseEvents');
        clickEvent.initEvent(eventType, true, true);
        targetNode.dispatchEvent(clickEvent);
    }
    ["mouseover", "mousedown", "mouseup", "click"].forEach(function(eventType) {
        triggerMouseEvent(targetNode, eventType);
    });
}
