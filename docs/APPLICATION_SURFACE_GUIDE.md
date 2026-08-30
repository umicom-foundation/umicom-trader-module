<!--
Umicom Trader Module
File: docs/APPLICATION_SURFACE_GUIDE.md

PURPOSE:
Explain how Trader adopts reusable Framework presentation components while
retaining safe simulation and the established GTK4 trading workstation.

AUTHOR AND ORGANISATION:
Sammy Hegab
Umicom Foundation

LICENCE:
MIT
-->

# Trader Application Surface

Trader now starts the Framework recipe
`org.umicom.workspace.trader.standard`. The recipe contains reusable chart,
watchlist, market depth, order ticket, orders, executions, portfolio, risk and
strategy components.

The application surface does not contain broker logic and cannot submit a live
order. Its order helper prepares a command labelled as simulation. The existing
Framework trading workspace still owns market state, order safeguards,
simulation, views and GTK4 rendering.

The GTK workstation creates the portable surface beside the existing trading
suite workstation. This lets both paths be inspected while migration continues,
without deleting tested behavior. A snapshot method exposes recipe and panel
state for diagnostics.

When adding a Trader panel:

1. Add or reuse a Framework trading component.
2. Add it to the Trader recipes in Framework.
3. Give it a portable presentation specification.
4. Implement market, pricing, risk or order behavior in Framework services.
5. Register a small Trader controller for product wording and command routing.
6. Keep live-trading permission and arming outside the presentation layer.

This separation keeps the visual application reusable and the trading safety
boundary clear.
