#set page(width: auto, height: auto, margin: 1cm)
#import "@preview/lilaq:0.6.0" as lq

#let tmp = csv("../../target/data/pendule_energie.csv").len()

#if tmp > 1 and tmp < 10000 {
  let (t, Ep, Ec, Em, _) = lq.load-txt(
    read("../../target/data/pendule_energie.csv"),
    delimiter: ";",
    skip-rows: 1,
    converters: x => x,
  )
  let t = t.map(x => float(x))
  let Ep = Ep.map(x => float(x))
  let Ec = Ec.map(x => float(x))
  let Em = Em.map(x => float(x))

  figure(
    lq.diagram(
      legend: (dx: 20mm, position: horizon + right),
      lq.plot(t, Ep, smooth: true, mark: none, label: $E_p$),
      lq.plot(t, Ec, smooth: true, mark: none, label: $E_c$),
      lq.plot(t, Em, smooth: true, mark: none, label: $E_m$),

      lq.xaxis(
        label: $t$,
      ),
      lq.yaxis(label: none)
    ),
    caption: [Énergie cinétique ($E_c$), potentiel ($E_p$) et mécanique ($E_m$), au cours du temps],
  )
}