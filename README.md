# Umicom Trader Module

Thin C23 application composition for the Umicom Trader product.

The module intentionally contains **no duplicated trading, market-data, risk,
selection-routing or linked-workbench logic**. Those reusable capabilities live
in Umicom Framework. This first module phase consumes the Framework-owned
Trader/TMS workbench profile and exposes it as a product-specific composition.

Current composition:

- Trading group: `trading.red`
- Operations group: `operations.green`
- Watchlist
- Chart
- Order Entry
- Account Selector
- Risk
- Trade Blotter
- Context Inspector

Trader also starts the Framework-owned standard presentation recipe with chart,
watchlist, depth, order, execution, portfolio, risk and strategy components.
The order helper prepares simulation state only; it cannot submit a live order.
See [the Trader application surface guide](docs/APPLICATION_SURFACE_GUIDE.md).
The [Trader runtime behavior and safety guide](docs/RUNTIME_BEHAVIOR_AND_TRADING_SAFETY.md)
explains full-rate market panels, guarded order commands, shared instrument
context and frequent workspace checkpoints.

Future Trader work can add product-specific presentation and broker adapters,
while canonical instruments, accounts, trades, risk, safeguards and context
routing remain Framework responsibilities.
