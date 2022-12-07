from plotnine import *
import pandas as pd

def strong():
    df = pd.read_csv("data.out")
    df = df.astype({"Grid Size": "category", "Ranks": "category"})
    # df2 = pd.DataFrame([
    #     {"Grid Size": 64, "Time": .037},
    #     {"Grid Size": 128, "Time": .22},
    #     {"Grid Size": 256, "Time": 1.57},
    #     {"Grid Size": 512, "Time": 12},
    #     {"Grid Size": 1024, "Time": 130}])
    # + geom_hline(data=df2, mapping=aes(yintercept="Time"))\
    plot = ggplot(df, aes(x="Threads", y="Time", color="Ranks")) + facet_wrap("Grid Size", nrow=2, scales="free") \
           + geom_line()\
           + theme(subplots_adjust={'wspace': 0.25, 'hspace': .25})
    plot.save("strong_scaling.png")


if __name__ == '__main__':
    strong()
