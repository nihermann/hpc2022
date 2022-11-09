from plotnine import *
import pandas as pd

def strong():
    df = pd.read_csv("slurm-1441825.out")
    df = df.astype({"Grid Size": "category"})
    plot = ggplot(df, aes(x="Threads", y="Time", color="Grid Size")) + facet_wrap("Grid Size", nrow=2, scales="free") + geom_point() + theme(subplots_adjust={'wspace': 0.25, 'hspace': .25})
    plot.save("strong_scaling.png")


def weak():
    df = pd.read_csv("slurm-1442734.out")
    df = df.astype({"Grid Size": str})
    plot = ggplot(df, aes(x="Threads", y="Time")) + geom_line() # + theme(subplots_adjust={'wspace': 0.25, 'hspace': .25})
    plot.save("weak_scaling.png")

if __name__ == '__main__':
    strong()
    weak()