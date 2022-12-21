from plotnine import *
import pandas as pd

def weak():
    df = pd.read_csv("slurm-1550006.out")
    df = df.astype({"nJobs": str})
    plot = ggplot(df, aes(x="Processes", y="Time", color="nJobs")) + geom_line() # + theme(subplots_adjust={'wspace': 0.25, 'hspace': .25})
    plot.save("jobs.png")

if __name__ == '__main__':
    weak()