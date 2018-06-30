let clickRecoder = {}
let clickSet = new Set() // For click action

let G = new jsnx.MultiDiGraph()
let unG
let subG
let dataset
let sorted_node
let subnodes
let pathnodes
let edgeSet = new Set()
let draw_function = new Draw_Force(800, 600)

function loadJsonFile () {
  d3.json(getFilePath()).then(function (data) {
    clickSet.clear()
    updateClickList()

    console.log('loadJsonFile:', 'Successful get data from path:', getFilePath(), data)
    dataset = data

    G.clear()
    edgeSet.clear()
    let t1 = data.head.vars[0]
    let p1 = data.head.vars[1]
    let t2 = data.head.vars[2]

    for (edge of data.results.bindings) {
      let sub = uriToStr(edge[t1].value)
      let pre = uriToStr(edge[p1].value)
      let obj = uriToStr(edge[t2].value)

      edgeSet.add(pre)

      G.addEdge(edge[t1].value, edge[t2].value, {label: pre, id: sub + '-' + pre + '-' + obj, type: edge[p1].value})
      G.node.get(edge[t1].value).type = edge[t1].type
      G.node.get(edge[t1].value).label = sub

      G.node.get(edge[t2].value).type = edge[t2].type
      G.node.get(edge[t2].value).label = obj
    }
    let bc = jsnx.betweennessCentrality(G)
    for (node of G) {
      G.node.get(node).degree = G.degree(node)
      G.node.get(node).inDegree = G.inDegree(node)
      G.node.get(node).outDegree = G.outDegree(node)
      G.node.get(node).betweennessCentrality = bc.get(node)
    }

    // draw(G)
    show_graph_info()
    draw_function = new Draw_Force(800, 600)
    updateSubgraph(getNodeNumber())
  }).catch(error =>
    console.error('loadJsonFile():', 'Fail get data cause by', error))
}

function display_data (data) {
  clickSet.clear()
  updateClickList()

  console.log('loadJsonFile:', 'Successful get data url:')
  set_status_info('Create graph.....')
  dataset = data
  G.clear()
  edgeSet.clear()

  let t1 = data.head.vars[0]
  let p1 = data.head.vars[1]
  let t2 = data.head.vars[2]

  for (edge of data.results.bindings) {
    let sub = uriToStr(edge[t1].value)
    let pre = uriToStr(edge[p1].value)
    let obj = uriToStr(edge[t2].value)

    edgeSet.add(pre)

    G.addEdge(edge[t1].value, edge[t2].value, {label: pre, id: sub + '-' + pre + '-' + obj, type: edge[p1].value})
    G.node.get(edge[t1].value).type = edge[t1].type
    G.node.get(edge[t1].value).label = sub

    G.node.get(edge[t2].value).type = edge[t2].type
    G.node.get(edge[t2].value).label = obj
  }
  let bc = jsnx.betweennessCentrality(G)
  for (node of G) {
    G.node.get(node).degree = G.degree(node)
    G.node.get(node).inDegree = G.inDegree(node)
    G.node.get(node).outDegree = G.outDegree(node)
    G.node.get(node).betweennessCentrality = bc.get(node)
  }

  // draw(G)
  show_graph_info()
  draw_function = new Draw_Force(800, 600)
  updateSubgraph(getNodeNumber())
}

function changeNodeNumber () {
  let nodeNb = document.getElementById('nodeNumber').value
  updateSubgraph(getNodeNumber(), getIndicator())
}

function updateSubgraph (nodeNb, indicator) {
  console.log('updateSubgraph()', 'nodeNb:', nodeNb, 'indicator', indicator)
  if (nodeNb <= 0) {
    draw_function.updateGraph(G)
    return
  }

  set_status_info('Sort the nodes.....')

  if (indicator === 'degree') {
    sorted_node = G.nodes(true).sort(function (a, b) {
      return b[1].degree - a[1].degree
    })
  } else if (indicator === 'in_degree') {
    sorted_node = G.nodes(true).sort(function (a, b) {
      return b[1].inDegree - a[1].inDegree
    })
  } else if (indicator === 'out_degree') {
    sorted_node = G.nodes(true).sort(function (a, b) {
      return b[1].outDegree - a[1].outDegree
    })
  } else {
    sorted_node = G.nodes(true).sort(function (a, b) {
      return b[1].betweennessCentrality - a[1].betweennessCentrality
    })
  }

  subnodes = []
  for (node of sorted_node.slice(0, nodeNb)) {
    subnodes.push(node[0])
  }

  unG = new jsnx.Graph(G)

  pathnodes = new Set([])

  for (var i = 0; i < subnodes.length; i++) {
    for (var j = i + 1; j < subnodes.length; j++) {
      if (jsnx.hasPath(unG, {source: subnodes[i], target: subnodes[j]})) {
        jsnx.shortestPath(unG, {source: subnodes[i], target: subnodes[j]}).forEach(pathnodes.add, pathnodes)
      } else {
        pathnodes.add(subnodes[i], subnodes[j])
      }
    }
  }

  subG = G.subgraph(pathnodes)

  draw_function.updateGraph(subG)
}

let server = 'http://127.0.0.1:5000/summary_graph?'
let summary_graph_data

function submit_keyword () {
  clickSet.clear()
  updateClickList()

  remove_graph_info()
  let k = document.getElementById('input_distance').value
  let keyword = document.getElementById('input_keyword').value
  let database = getDBName()

  console.log('keyword', keyword, 'distance', k, 'database', database
  )
  if (keyword === '') {
    alert('Please type a key word')
    return false
  }
  if (k < 1) {
    alert('The distance have to bigger than 1')
    return false
  }

  set_status_info('Submit info to server......:' +
    'key word:' + keyword +
        ' distance: ' + k +
    ' database：' + database)

  let url = server + 'keyword=' + keyword + '&k=' + k + '&database=' + database
  console.log('Submit_keyword()', url)
  d3.json(url).then(data => {
    set_status_info('Successfully Get data...')
    console.log('Update data', data)
    summary_graph_data = data
    json_to_graph(data)

    updateSubgraph(getNodeNumber())
  }).catch(error => {
    set_status_info('Error：' + error)
    console.log('Get error:', error)
  })
  return true
}

function json_to_graph (data) {
  set_status_info('Transform json to graph....')
  G.clear()
  edgeSet.clear()

  for (let node of data.nodes) {
    node.label = uriToStr(node.id)
    G.addNode(node.id, node)
  }

  for (let edge of data.links) {
    edge.label = uriToStr(edge.id)
    edgeSet.add(edge.label)
    G.addEdge(edge.source, edge.target, edge)
  }

  set_status_info('Calcule the degree ....')

  let bc = jsnx.betweennessCentrality(G)
  for (node of G) {
    G.node.get(node).degree = G.degree(node)
    G.node.get(node).inDegree = G.inDegree(node)
    G.node.get(node).outDegree = G.outDegree(node)
    G.node.get(node).betweennessCentrality = bc.get(node)
  }
  show_graph_info()
}

function show_graph_info () {
  document.getElementById('graph_info').innerHTML = jsnx.info(G).replace(/\n/g, '<br />')
}

function set_status_info (info) {
  document.getElementById('status_info').innerHTML = info
}

function remove_graph_info () {
  document.getElementById('graph_info').innerHTML = ''
}

function uriToStr (uri) {
  uri = uri.replace(/<|>|"/g, '')
  if (uri.includes('http')) {
    let val = uri.split(/#|\//)
    return val[val.length - 1]
  }

  return uri
}

function hasDisplayEdgeLable () {
  let checked = document.getElementById('show_link_label').checked
  if (checked) {
    d3.selectAll('.edgelabel').attr('display', 'block')
  } else {
    d3.selectAll('.edgelabel').attr('display', 'none')
  }

  console.log('hasDisplayEdgeLable()', checked)
}

function hasDisplayNodeLable () {
  let checked = document.getElementById('show_node_label').checked
  if (checked) {
    d3.selectAll('.nodeLabel').attr('display', 'block')
  } else {
    d3.selectAll('.nodeLabel').attr('display', 'none')
  }

  console.log('hasDisplayNodeLable()', checked)
}
