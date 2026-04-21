#set page(width: auto, height: auto, margin: 1cm)
#import "@preview/lilaq:0.6.0" as lq

#let tmp = csv("../../target/data/pendule_portrait.csv").len()

#if tmp > 1 and tmp < 10000 {

  let (.., th1, o1, _) = lq.load-txt(
    read("../../target/data/pendule_portrait.csv"),
    delimiter: ";",
    skip-rows: 1,
    converters: x => x,
  )
  let th1 = th1.map(x => float(x))
  let o1 = o1.map(x => float(x))


  figure(
    lq.diagram(
      legend: (dx: 20mm, position: horizon + right),
      lq.plot(o1, th1, smooth: true, mark: none),
    ),
    caption: [portrait de phase de $theta_n, omega_n$],
  )
}