<!--
Umicom Trader Module
File: docs/RUNTIME_BEHAVIOR_AND_TRADING_SAFETY.md

PURPOSE:
Explain Trader's adoption of shared runtime policy and its command safety rules.

AUTHOR AND ORGANISATION:
Sammy Hegab
Umicom Foundation

LICENCE:
MIT
-->

# Trader Runtime Behavior and Trading Safety

Trader has panels that change much more often than a normal business form. A
chart, watchlist and market-depth view may receive continuous simulated or live
data. An order ticket can prepare a command that must never be treated like a
simple display action.

Umicom Framework now describes those differences as reusable behavior.

## Market panels

Streaming panels declare a one-second scheduling interval. The frontend calls
`umi_trader_gtk_workstation_advance()` with real elapsed time. The Framework
then refreshes the panels that are due.

The standard Trader recipe keeps scheduled work at full rate in the background
and asks for a workspace checkpoint every 15 seconds. This is a presentation
policy only. Market-data transports continue to follow their own connection,
entitlement and recovery rules.

## Shared instrument context

Chart, watchlist, depth, order and risk panels share the
`umicom.context.instrument` group. A product adapter can pass a selected symbol
through `umi_trader_gtk_workstation_context_changed()`. Compatible panels react
without Trader copying selection-routing code.

## Guarded order commands

The order ticket is marked `guarded`. A guarded command is rejected unless
Trader has registered a controller for that component. Trader does register its
simulation-safe controller during surface creation.

This gate is deliberately additive. It does not replace the established order
validation, risk, permission, approval, broker or live-arming checks. Those
domain safeguards remain mandatory before any future live submission path.

## Frontend responsibilities

The GTK4 frontend should:

1. pass elapsed time to the workstation;
2. report foreground and background transitions;
3. publish selected-instrument context;
4. save a checkpoint when the policy reports that one is due;
5. continue to use trading-domain services for every order decision.

The Framework owns the common scheduling and policy rules. Trader owns only
product meaning, safe simulation behavior and future broker-specific adapters.
