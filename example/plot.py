#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Example cluster-connection plot."""
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


df = pd.read_csv("cluster-connections.csv")

r = df.iloc[:, [0]]
partial_rdfs = np.array([df.iloc[:, [i]] for i in range(1, df.shape[1])])
total_rdf = partial_rdfs.sum(axis=0)

fig, ax = plt.subplots()
for i, rdf in enumerate(partial_rdfs):
    ax.plot(r, rdf, label=f"{i} atoms")
ax.plot(r, total_rdf, color="k", label="total")

ax.set_xlabel(r"r [$\AA$]")
ax.set_ylabel("RDF Si-Li")

ax.grid(linestyle=":")

ax.set_xlim((4, 8))
ax.set_ylim((0.0, 1.85))

ax.legend(title="cluster-connections")

fig.tight_layout()

fig.savefig("cluster-connections.png", dpi=200)
