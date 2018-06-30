let simulation

let Draw_Force = function (height, width) {
  let color = d3.scaleOrdinal(d3.schemeCategory20)
  let intervalTimer = null
  let links = null
  let nodes = null
  let link_labels = null
  let nodes_list
  let edges_list
  let unGraph

  this.height = height
  this.width = width

  this.graph = null

  // this.svg = d3.select("body").append('svg')
  this.svg = d3.select('#canvas')
    .attr('width', width)
    .attr('height', height)
  // .call(d3.zoom()
  //     .scaleExtent([.1, 4])
  //     .on("zoom", function() { this.svg.attr("transform", d3.event.transform); })
  // );

  const defs = this.svg.append('defs') // defs定义可重复使用的元素
    const arrowheads = defs.append('marker') // 创建箭头
    .attr('id', 'arrow')
    // .attr('markerUnits', 'strokeWidth') // 设置为strokeWidth箭头会随着线的粗细进行缩放
    .attr('markerUnits', 'userSpaceOnUse') // 设置为userSpaceOnUse箭头不受连接元素的影响
    .attr('class', 'arrowhead')
    .attr('markerWidth', 26) // viewport
    .attr('markerHeight', 26) // viewport
    .attr('viewBox', '0 0 20 20') // viewBox
    .attr('refX', 18) // 偏离圆心距离
    .attr('refY', 5) // 偏离圆心距离
    .attr('orient', 'auto') // 绘制方向，可设定为：auto（自动确认方向）和 角度值
    
    arrowheads.append('path')
    .attr('d', 'M0,0 L0,10 L10,5 z') // d: 路径描述，贝塞尔曲线
    .attr('fill', 'DarkCyan') // 填充颜色

    this.links_group = this.svg.select('#links')

  this.nodes_group = this.svg.select('#nodes')

  this.labels_group = this.svg.select('#edgeLabels')

  simulation = d3.forceSimulation()
    .force('link', d3.forceLink().distance(200).id(function (d) { return d.id }))
    .force('charge', d3.forceManyBody().distanceMin(5).distanceMax(100).strength(-60))
    .force('center', d3.forceCenter(width / 2, height / 2))

    this.updateScale = function (graph) {
    let maxD = d3.max(nodes_list, d => d.degree)
        let minD = d3.min(nodes_list, d => d.degree)
        let maxInD = d3.max(nodes_list, d => d.inDegree)
        let minInD = d3.min(nodes_list, d => d.inDegree)
        let maxOutD = d3.max(nodes_list, d => d.outDegree)
        let minOutD = d3.min(nodes_list, d => d.outDegree)

        //console.log('min:', min, 'max', max);
        scaleD = d3.scaleLinear().range([6, 20])
      .domain([Math.sqrt(minD), Math.sqrt(maxD)])
    scaleInD = d3.scaleLinear().range([6, 20])
      .domain([Math.sqrt(minInD), Math.sqrt(maxInD)])
    scaleOutD = d3.scaleLinear().range([6, 20])
      .domain([Math.sqrt(minOutD), Math.sqrt(maxOutD)])
  }

  this.updateGraph = function (graph) {
    this.graph = graph
    unGraph = new jsnx.Graph(graph)

    nodes_list = []
    for (let g_node of graph.nodes(true)) {
      g_node[1].id = g_node[0]
      nodes_list.push(g_node[1])
    }

    // console.log("nodes_list", nodes_list)
    edges_list = []
    for (let g_edge of graph.edges(true)) {
      edges_list.push({
        source: g_edge[0],
        target: g_edge[1],
        label: g_edge[2].label,
        id: g_edge[2].id
      })
    }
    // console.log("edges_list", edges_list)

    // console.log("updateGraph():Get graph" + graph)
    this.updateScale(graph)
    links = this.links_group.selectAll('.link')
      .data(edges_list, d => {
        console.log('edge', d)
        return d.id
      })

    links.exit()
    // .transition()
    // .duration(300)
    // .attr("stroke-width", 0)
      .remove()
        
        links = links.enter().append('path')
      .attr('id', d => {
        console.log('links', d)
        return d.id
      })
      .attr('class', 'link')
      .attr('stroke', 'LightSeaGreen')
      .attr('stroke-width', 2)
      .attr('marker-end', 'url(#arrow)')
      .merge(links)

    link_labels = this.labels_group.selectAll('.edgelabel').data(edges_list, d => d.id)

    link_labels.enter().append('text')
      .attr('class', 'edgelabel')
    // .append('textPath')
    // .attr("xlink:herf", d=> "#"+d.id)
      .text(d => d.label)

    link_labels.exit().transition().duration(1000).attr('opacity', 0).remove()

    nodes = this.nodes_group.selectAll('.node_group').data(nodes_list, d => {
      // TODO
      console.log('node', d)
      return d.id
    })
    let nodes_enter = nodes.enter().append('g').attr('class', 'node_group').attr('id', d => d.id)

    nodes_enter.append('circle').attr('r', d => {
      return scaleD(Math.sqrt(d.degree))
    })
      .attr('stroke', 'SeaGreen')
      .attr('stroke-width', 2)
    // .attr("fill", function(d) { return color(d.group); })
      .attr('fill', function (d) {
        if (d.type === 'uri') {
          return 'SandyBrown'
        } else if (d.type === 'bnode') {
          return 'SkyBlue'
        } else {
          return 'Pink'
        }
      })
      .on('mouseover', focus)
      .on('mouseout', unfocus)
      .on('click', onClickNode)
    // .on("dblclick", dragend)
      .call(d3.drag()
        .on('start', dragstart)
        .on('drag', drag))
    // .on("end", dragended)
    // .on("start", dragstart))

    nodes_enter.append('title')
      .text(function (d) { return d.id })
        nodes_enter.append('text').text(d => d.label)
      .attr('class', 'nodeLabel')
      .attr('dy', d => 2 * scaleD(Math.sqrt(d.degree)))
      .attr('dx', d => 2 * scaleD(Math.sqrt(d.degree)))


    let nodes_exit = nodes.exit()

    nodes_exit.selectAll('circle')
      .transition().duration(1000)
      .attr('opacity', 0)
      .attr('r', 0)
    // .attr("fill", function(d) { return color(d.group); })
      .remove()

        nodes_exit.selectAll('text')
      .transition().duration(1000)
      .attr('opacity', 0)
    // .attr("fill", function(d) { return color(d.group); })
      .remove()
        nodes_exit.remove()

    nodes = this.nodes_group.selectAll('.node_group').data(nodes_list, d => d.id)
    link_labels = d3.selectAll('.edgelabel').data(edges_list, d => d.id)

    simulation
      .nodes(nodes_list)
      .on('tick', tick)

        simulation.force('link')
      .links(edges_list)

        simulation.alphaTarget(0.1).restart()
        set_status_info('Successfully draw the picture')
    hasDisplayEdgeLable()
    hasDisplayNodeLable()

    let zoom = d3.zoom().scaleExtent([0.2, 10]).on('zoom', zoomed)

    d3.select('#canvas')
      .call(zoom)
      .on('dblclick.zoom', null)

    }

  function zoomed () {
    d3.select('#links').attr('transform',
      'translate(' + d3.event.transform.x + ',' + d3.event.transform.y + ')scale(' + d3.event.transform.k + ')')

    d3.select('#nodes').attr('transform',
      'translate(' + d3.event.transform.x + ',' + d3.event.transform.y + ')scale(' + d3.event.transform.k + ')')

    d3.select('#edgeLabels').attr('transform',
      'translate(' + d3.event.transform.x + ',' + d3.event.transform.y + ')scale(' + d3.event.transform.k + ')')
  }

  // this.edges_list.forEach(function(d) {
  //     adjlist[d.source.index + "-" + d.target.index] = true;
  //     adjlist[d.target.index + "-" + d.source.index] = true;
  // });

  function neigh (a, b) {
    // console.log("a ", a," b ", b,unGraph.neighbors(b), (a in unGraph.neighbors(b)))

    return (unGraph.adj.get(b).get(a) !== undefined) || (a === b)
    }

  function focus (d) {
    var id = d3.select(d3.event.target).datum().id
        nodes.style('opacity', function (o) {
      return neigh(id, o.id) ? 1 : 0.1
        })

        links.style('opacity', function (o) {
      return o.source.id == id || o.target.id == id ? 1 : 0.1
        })
    }

  function unfocus () {
    nodes.style('opacity', 1)
       links.style('opacity', 1)
    }

  function dragstart (d) {
    if (!d3.event.active) simulation.alphaTarget(0.1).restart()
        d.fx = d.x
        d.fy = d.y
    }

  function drag (d) {
    console.log('drag()', '拖拽')
    d.fx = d3.event.x
        d.fy = d3.event.y
        let nodeColor = d3.select(this).attr('fill')
        if (nodeColor == 'SandyBrown') {
      d3.select(this).attr('fill', 'Tomato')
    } else if (nodeColor == 'SkyBlue') {
      d3.select(this).attr('fill', 'RoyalBlue')
    }

    clickSet.add(d.id)
    updateClickList()
  }
  //
  // function dragend(d) {
  //     //clearTimeout(intervalTimer);
  //     // dblclick 事件的处理
  //     console.log("释放")
  //     if (!d3.event.active) simulation.alphaTarget(0);
  //     d.fx = null;
  //     d.fy = null;
  //     let nodeColor = d3.select(this).attr('fill');
  //     if (nodeColor == "Tomato") {
  //         d3.select(this).attr('fill', "SandyBrown")
  //     } else if (nodeColor == "RoyalBlue") {
  //         d3.select(this).attr('fill', "SkyBlue")
  //     }
  // }

  function onClickNode (d) {
    if (d3.event.defaultPrevented) return

        if (window.event.ctrlKey) {
      if (d.id in clickRecoder) {
        pathnodes = new Set([...pathnodes].filter(x => !clickRecoder[d.id].has(x)))
        delete clickRecoder[d.id]
      } else{
        let clickAction = getClickAction()
        let newNodesList
        if (clickAction === 'neighbors') {
          console.log('neighbors')
          newNodesList = G.neighbors(d.id)
        } else{
          newNodesList = unG.neighbors(d.id)
        }

        let difference = new Set([...newNodesList].filter(x => !pathnodes.has(x)))
        clickRecoder[d.id] = difference
        pathnodes = new Set([...pathnodes, ...difference])
      }

      subG = G.subgraph(pathnodes)
      draw_function.updateGraph(subG)

      let nodeColor = d3.select(this).attr('stroke')

            if (nodeColor == 'SeaGreen') {
        d3.select(this)
          .attr('stroke', 'MediumPurple')
          .attr('stroke-width', 5)
      } else if (nodeColor == 'MediumPurple') {
        d3.select(this)
          .attr('stroke', 'SeaGreen')
          .attr('stroke-width', 2)
      }
    } else{
      console.log('释放', d)
      if (!d3.event.active) simulation.alphaTarget(0)
            d.fx = null
            d.fy = null
            let nodeColor = d3.select(this).attr('fill')
            if (nodeColor == 'Tomato') {
        d3.select(this).attr('fill', 'SandyBrown')
      } else if (nodeColor == 'RoyalBlue') {
        d3.select(this).attr('fill', 'SkyBlue')
      }

      if (clickSet.has(d.id)) {
        clickSet.delete(d.id)
      }

      updateClickList()
    }
  }

  function tick () {
    links
      .attr('d',
        (d) =>
        {
 return d && 'M '
            + d.source.x + ' '
            + d.source.y + ' L ' + d.target.x + ' ' + d.target.y })

    nodes
      .attr('transform', function (d) {
        return 'translate(' + d.x + ',' + d.y + ')';
      })

    link_labels.attr('transform', function (d) { // calcul de l'angle du label
      var angle = Math.atan((d.source.y - d.target.y) / (d.source.x - d.target.x)) * 180 / Math.PI
            return 'translate(' + [((d.source.x + d.target.x) / 2), ((d.source.y + d.target.y) / 2)] + ')rotate(' + angle + ')'
        })

    }

  // function dblclick(d) {
  //   d3.select(this).classed("fixed", d.fixed = false);
  // }

  // function dragstart(d) {
  //   d3.select(this).classed("fixed", d.fixed = true);
  // }

  d3.select('#back').on('click', function () {
    d3.json('data/total_G.json').then(
      data => {
        updateGraph(data)
      })
  })


    function collide (node) {
    var r = node.radius + 16,
      nx1 = node.x - r,
      nx2 = node.x + r,
      ny1 = node.y - r,
      ny2 = node.y + r
        return function (quad, x1, y1, x2, y2) {
      if (quad.point && (quad.point !== node)) {
        var x = node.x - quad.point.x,
          y = node.y - quad.point.y,
          l = Math.sqrt(x * x + y * y),
          r = node.radius + quad.point.radius
                if (l < r) {
          l = (l - r) / l * 0.5
                    node.x -= x *= l
                    node.y -= y *= l
                    quad.point.x += x
                    quad.point.y += y
                }
      }
      return x1 > nx2 || x2 < nx1 || y1 > ny2 || y2 < ny1
        }
  }
}

function updateClickList () {
  console.log('update List:', clickSet)
  let ckList = d3.select('#clickList').selectAll('li').data([...clickSet], d => {
    return d
  })

  ckList.enter().append('li').attr('class', 'list-group-item').text(d => d)

  ckList.exit().remove()
}

function getSubgrah () {
  subnodes = [...clickSet]
  pathnodes = new Set()
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
