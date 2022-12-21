from plotnine import *
import pandas as pd

def strong():
    df = pd.read_csv("strong.out")
    df2 = pd.read_csv("strong2.out")
    df["Distributed"] = False
    df2["Distributed"] = True
    df = pd.concat([df, df2])
    df = df.astype({"Grid Size": "category"})
    plot = ggplot(df, aes(x="Processes", y="Rate", color="Distributed")) \
           + facet_wrap("Grid Size", nrow=2, scales="free") \
           + geom_line() + theme(subplots_adjust={'wspace': 0.35, 'hspace': .25})
    plot.save("strong_scaling3.png")


def weak():
    df = pd.read_csv("weak.out")
    df2 = pd.read_csv("weak2.out")
    df2 = df2[df2["Processes"]<17]
    df["Distributed"] = False
    df2["Distributed"] = True
    df = pd.concat([df, df2])
    df = df.astype({"Grid Size": str})
    plot = ggplot(df, aes(x="Processes", y="Rate", color="Distributed")) + geom_line() # + theme(subplots_adjust={'wspace': 0.25, 'hspace': .25})
    plot.save("weak_scaling3.png")

if __name__ == '__main__':
    strong()
    weak()