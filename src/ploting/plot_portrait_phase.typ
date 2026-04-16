#set page(width: auto, height: auto, margin: 1cm)
#import "@preview/lilaq:0.6.0" as lq

#let (th0, o0, th1, o1, _) = lq.load-txt(
  read("../../target/penduleportrait.csv"),
  delimiter: ";",
  skip-rows: 1,
  converters: x => x,
)
#let th0 = th0.map(x => float(x))
#let o0 = o0.map(x => float(x))
#let th1 = th1.map(x => float(x))
#let o1 = o1.map(x => float(x))

#figure(
  lq.diagram(
    legend: (dx: 20mm, position: horizon + right),
    lq.plot(o0, th0, smooth: true, mark: none),
  ),
  caption: [portrait de phase de $theta_1, omega_1$],
)

#figure(
  lq.diagram(
    legend: (dx: 20mm, position: horizon + right),
    lq.plot(o1, th1, smooth: true, mark: none),
  ),
  caption: [portrait de phase de $theta_2, omega_2$],
)
