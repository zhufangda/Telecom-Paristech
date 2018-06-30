
/* selectected activities fro time spent */
var selectedActivitiesTs = []

var keyword_li = document.getElementById('keyword_li')
var query_li = document.getElementById('query_li')
var file_li = document.getElementById('file_li')

window.onload = init_index_html

keyword_li.addEventListener('click', function () {
  keyword_li.setAttribute('class', 'active')
  query_li.classList.remove('active')
  file_li.classList.remove('active')

  document.getElementById('div-dropdown-keyword')
    .setAttribute('style', 'display:block')
  document.getElementById('div-dropdown-query')
    .setAttribute('style', 'display:none')
  document.getElementById('div-dropdown-file')
    .setAttribute('style', 'display:none')
})

query_li.addEventListener('click', function () {
  query_li.setAttribute('class', 'active')
  keyword_li.classList.remove('active')
  file_li.classList.remove('active')

  document.getElementById('div-dropdown-keyword').setAttribute('style', 'display:none')
  document.getElementById('div-dropdown-query').setAttribute('style', 'display:block')
  document.getElementById('div-dropdown-file').setAttribute('style', 'display:none')
})

file_li.addEventListener('click', function () {
  file_li.setAttribute('class', 'active')
  keyword_li.classList.remove('active')
  query_li.classList.remove('active')

  document.getElementById('div-dropdown-keyword').setAttribute('style', 'display:none')
  document.getElementById('div-dropdown-query').setAttribute('style', 'display:none')
  document.getElementById('div-dropdown-file').setAttribute('style', 'display:block')
})

function hasNodeLabel () {
  return $('#show_node_label')[0].checked
}

function hasEdgeLabel () {
  return $('#show_link_label')[0].checked
}

/** Get the click action.
 * return: retrun "neigbors" if  "show neighbors is checked", else return "relative"***/
function getClickAction () {
  return document.querySelector('input[name="click_action"]:checked').value
}

function getIndicator () {
  return document.querySelector('input[name="show_indicator"]:checked').value
}

function getNodeNumber () {
  return document.getElementById('nodeNumber').value
}

function getFilePath () {
  return document.getElementById('input_file_path').value
}

function getDBName () {
  return document.querySelector('input[name="databaseList"]:checked').value
}

/* returns all the checked activities among the selected activities */
function get_checked_activities () {
  var checked_activities = []
  for (var i = 0; i < selectedActivitiesTs.length; i++) {
    if (selectedActivitiesTs[i].children[1].children[0].checked) { checked_activities.push(selectedActivitiesTs[i].children[0].innerText) }
  }

  return checked_activities
}

/* creates a pop-up div to zoom on a element */
function zoom_on_element (id) {

}

function init_index_html () {
  document.getElementById('div-dropdown-keyword')
    .setAttribute('style', 'display:none')

  document.getElementById('div-dropdown-query')
    .setAttribute('style', 'display:none')

  var button = document.getElementById('button_activity_pt')
}
