from plotnine import *
import pandas as pd

def strong():
    df = pd.read_csv("slurm-1441825.out")
    df = df.astype({"Grid Size": "category"})
    df2 = pd.DataFrame([
        {"Grid Size": 64, "Time": .037},
        {"Grid Size": 128, "Time": .22},
        {"Grid Size": 256, "Time": 1.57},
        {"Grid Size": 512, "Time": 12},
        {"Grid Size": 1024, "Time": 130}])
    plot = ggplot(df, aes(x="Threads", y="Time", color="Grid Size")) + facet_wrap("Grid Size", nrow=2, scales="free") \
           + geom_line() + geom_hline(data=df2, mapping=aes(yintercept="Time"))\
           + theme(subplots_adjust={'wspace': 0.25, 'hspace': .25})
    plot.save("strong_scaling.png")


def weak():
    df = pd.read_csv("slurm-1442734.out")
    df = df.astype({"Grid Size": str})
    plot = ggplot(df, aes(x="Threads", y="Time")) + geom_line() # + theme(subplots_adjust={'wspace': 0.25, 'hspace': .25})
    plot.save("weak_scaling.png")

if __name__ == '__main__':
    strong()
    weak()