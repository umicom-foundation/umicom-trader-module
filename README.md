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

Trader also starts Framework-owned learning, standard or focus presentation recipes with chart,
watchlist, depth, order, execution, portfolio, risk and strategy components.
The order helper prepares simulation state only; it cannot submit a live order.
See [the Trader application surface guide](docs/APPLICATION_SURFACE_GUIDE.md).
The [Trader runtime behavior and safety guide](docs/RUNTIME_BEHAVIOR_AND_TRADING_SAFETY.md)
explains full-rate market panels, guarded order commands, shared instrument
context and frequent workspace checkpoints.
The [workstation capability map](docs/TRADING_WORKSTATION_CAPABILITY_MAP.md)
separates available, foundational and planned workstation behaviour.

The graphical workstation uses the Framework appearance editor. A user can
choose dark, light, high-contrast, retro or neo styling, or create a custom
profile with their own fonts, text scale, control density and semantic
colours. The window remains resizable and layout editing continues to control
docking, floating, grouping and linked panel context independently of theme.

The executable, taskbar entry and installed desktop entry use the canonical
Umicom `<>` icon. The product name remains native text, so it stays clear and
accessible at different display scales.

Trader's public module headers now use the same complete file comment and
unique-guard audit as Framework and Studio, keeping the product SDK predictable
for future frontend and broker adapters.

Future Trader work can add product-specific presentation and broker adapters,
while canonical instruments, accounts, trades, risk, safeguards and context
routing remain Framework responsibilities.

The Windows suite installer exposes Trader as the optional **Trader**
component. Its graphical executable is registered with Umicom Desk, allowing
Trader and Studio or another installed product to run side by side.
