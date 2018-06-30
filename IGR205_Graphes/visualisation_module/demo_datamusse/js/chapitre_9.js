let dataset = []
let minX, maxX
let width = 800
let height = 600
let padding = 1
let xPadding = 1
let yPadding = 1
let scaleX, scaleC, scaleH
let n = 20
let svg

function init () {
  genData()
  updateScale()
  svg = d3
    .select('body')
    .append('svg')
    .attr('width', width + 2 * xPadding)
    .attr('height', height + 2 * yPadding)

  svg
    .selectAll('rect')
    .data(dataset)
    .enter()
    .append('rect')
    .attr('width', scaleX.bandwidth())
    .attr('x', (d, i) => scaleX(i))
    .attr('height', d => scaleH(d))
    .attr('transform', d => 'translate(0,' + (height - scaleH(d)) + ')')
    .attr('fill', d => 'rgb(0,0, ' + scaleC(d) + ')')

  svg
    .selectAll('text')
    .data(dataset)
    .enter()
    .append('text')
    .attr('x', (d, i) => scaleX(i) + scaleX.bandwidth() / 2)
    .attr('y', d => height - scaleH(d) + 16)
    .attr('font-size', '12')
    .attr('font-family', 'sans-serif')
    .attr('text-anchor', 'middle')
    .attr('fill', 'white')
    .text(d => d)
}

function addData () {
  dataset.push(Math.ceil(Math.random) * 25)
  n = dataset.length
}

function updateScale () {
  minX = d3.min(dataset)
  maxX = d3.max(dataset)

  scaleX = d3.scaleBand()
    .domain(d3.range(dataset.length))
    .rangeRound([0, width])
    .paddingInner(0.05)

  scaleH = d3.scaleLinear()
    .domain([0, maxX])
    .range([0, height])

  scaleC = d3.scaleLinear()
    .domain([minX, maxX])
    .range([10, 255])
}

function genData () {
  dataset = []
  for (let i = 0; i < n; i++) {
    dataset.push(Math.ceil(Math.random() * 25))
  }
}

function draw () {
  d3
    .selectAll('rect')
    .data(dataset)
    .transition()
    .delay((d, i) => {
      console.log('delay', i * 100)
      return i * 100
    })
    .duration(1000)
    .ease(d3.easeBounce)
    .attr('width', scaleX.bandwidth())
    .attr('x', (d, i) => scaleX(i))
    .attr('height', d => scaleH(d))
    .attr('transform', d => 'translate(0,' + (height - scaleH(d)) + ')')
    .attr('fill', d => 'rgb(0,0, ' + scaleC(d) + ')')

  svg
    .selectAll('text')
    .data(dataset)
    .transition()
    .duration(1000)
    .delay((d, i) => {
      console.log('delay', i * 100)
      return i * 100
    })
    .ease(d3.easeElastic)
    .attr('x', (d, i) => scaleX(i) + scaleX.bandwidth() / 2)
    .attr('y', d => height - scaleH(d) + 16)
    .attr('font-size', '12')
    .attr('font-family', 'sans-serif')
    .attr('text-anchor', 'middle')
    .attr('fill', 'white')
    .text(d => d)
}

init()
/** 被选择的元素必须出现在 script代码出入的地方之前， 否则d3会找不到该元素 **/
d3.select('#update').on('click', function () {
  genData()
  updateScale()
  draw()
})

d3.select('#add').on('click', function () {
  console.log('Add data')
  addData()
  updateScale()
  svg.selectAll('rect')
    .data(dataset)
    .enter()
    .append('rect')
    .attr('width', scaleX.bandwidth())
    .attr('x', (d, i) => scaleX(i))
    .attr('height', d => scaleH(d))
    .attr('transform', d => 'translate(0,' + (height - scaleH(d)) + ')')
    .attr('fill', d => 'rgb(0,0, ' + scaleC(d) + ')')
})
