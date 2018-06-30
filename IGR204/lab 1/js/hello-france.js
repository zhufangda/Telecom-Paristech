alert('Hello')

const w = 800
const h = 800
let dataset = []
// 这是悬浮提示框
var div = d3.select('body').append('div')
  .attr('class', 'tooltip')
  .style('opacity', 0)

let svg = d3.select('body').append('svg').attr('width', w).attr('height', h)

function draw () {
  console.log('Draw picture')
  svg.selectAll('circle')
    .data(dataset)
    .enter()
    .append('circle')
  // .attr("width", 1)
  // .attr("height", 1)
    .attr('cx', (d) => x(d.longitude))
    .attr('cy', (d) => y(d.latitude))
    .style('opacity', (d) => gray(d.density))
    .attr('r', (d) => rayon(d.population))
    .attr('place', (d) => d.place)
    .attr('codePostal', (d) => d.codePostal)
    .on('mouseenter', showTip)
    .on('mousemove', showTip)

  svg.append('g')
    .attr('class', 'x axis')
    .attr('transform', 'translate(0, ' + h + ')')
    .call(d3.axisTop(x))
  // .call(() => console.log("B"));
}

d3.tsv('data/france.tsv')
  .row((d, i) => {
    return {
      codePostal: +d['Postal Code'],
      inseeCode: +d.inseecode,
      place: d.place,
      longitude: +d.x,
      latitude: +d.y,
      population: +d.population,
      density: +d.density
    }
  })
  .get((error, rows) => {
    console.log('Loaded ' + rows.length + ' rows')
    if (rows.length > 0) {
      dataset = rows
      x = d3.scaleLinear()
        .domain(d3.extent(rows, (row) => row.longitude))
        .range([0, w])
      y = d3.scaleLinear()
        .domain(d3.extent(rows, (row) => row.latitude))
        .range([h, 0])
      gray = d3.scaleLinear()
        .domain(d3.extent(rows, (row) => row.density))
        .range([0.2, 0.8])

      rayon = d3.scaleLinear()
        .domain(d3.extent(rows, (row) => row.population))
        .range([1, 30])

      setTimeout(function () {
        draw()
      }, 2000)
    }
  })

function showTip () {
  // 定义悬浮框的位置
  console.log('show', 'Local')
  div.html(setTip(this))
    .style('left', (d3.event.pageX) + 'px')
    .style('top', (d3.event.pageY - 28) + 'px')
  div.transition()
    .duration(300)
    .style('opacity', 0.9)
}

function setTip (obj) {
  return 'Name:\t' + obj.getAttribute('place') +
    '<br> Postal code\t' + obj.getAttribute('codePostal')
}

function hideTip () {
  console.log('hide', 'Local')
  div.transition()
    .duration(100)
    .style('opacity', 0)
}
