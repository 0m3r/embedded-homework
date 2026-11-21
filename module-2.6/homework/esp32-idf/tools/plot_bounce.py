#!/usr/bin/env python3

import argparse
import html
import re
from pathlib import Path

WAVE_RE = re.compile(r"WAVE,([^,]+),(\d+),([01])")
WIDTH = 1000
PANEL_HEIGHT = 130
LEFT = 90
RIGHT = 30
TOP = 45
WINDOW_US = 20_000
BURST_GAP_US = 100_000


def parse_log(path):
    channels = {}
    for line in path.read_text(errors="replace").splitlines():
        match = WAVE_RE.search(line)
        if not match:
            continue
        name, timestamp, state = match.groups()
        channels.setdefault(name, []).append((int(timestamp), int(state)))
    return channels


def split_bursts(edges):
    bursts = []
    for edge in edges:
        if not bursts or edge[0] - bursts[-1][-1][0] > BURST_GAP_US:
            bursts.append([edge])
        else:
            bursts[-1].append(edge)
    return bursts


def panel(name, number, edges, y_offset):
    plot_width = WIDTH - LEFT - RIGHT
    start = edges[0][0]
    duration = max(WINDOW_US, edges[-1][0] - start + 1_000)

    def x(timestamp):
        return LEFT + (timestamp - start) * plot_width / duration

    y_low = y_offset + 95
    y_high = y_offset + 55
    previous_state = 1 - edges[0][1]
    points = [(LEFT, y_high if previous_state else y_low)]

    for timestamp, state in edges:
        edge_x = x(timestamp)
        points.append((edge_x, points[-1][1]))
        points.append((edge_x, y_high if state else y_low))
    points.append((WIDTH - RIGHT, points[-1][1]))

    path_data = " ".join(
        ("M" if index == 0 else "L") + f"{point_x:.1f},{point_y:.1f}"
        for index, (point_x, point_y) in enumerate(points)
    )
    escaped_name = html.escape(name)
    elements = [
        f'<text x="{LEFT}" y="{y_offset + 22}" class="title">'
        f"{escaped_name} — burst {number}, {len(edges)} edge(s)</text>",
        f'<line x1="{LEFT}" y1="{y_low}" x2="{WIDTH - RIGHT}" '
        f'y2="{y_low}" class="grid"/>',
        f'<line x1="{LEFT}" y1="{y_high}" x2="{WIDTH - RIGHT}" '
        f'y2="{y_high}" class="grid"/>',
        f'<text x="{LEFT - 20}" y="{y_high + 5}" class="axis">1</text>',
        f'<text x="{LEFT - 20}" y="{y_low + 5}" class="axis">0</text>',
        f'<path d="{path_data}" class="signal"/>',
    ]

    for index in range(5):
        tick_us = duration * index / 4
        tick_x = LEFT + plot_width * index / 4
        elements.extend([
            f'<line x1="{tick_x:.1f}" y1="{y_low}" x2="{tick_x:.1f}" '
            f'y2="{y_low + 6}" class="tick"/>',
            f'<text x="{tick_x:.1f}" y="{y_low + 23}" class="axis" '
            f'text-anchor="middle">{tick_us / 1000:.1f} ms</text>',
        ])
    return "\n".join(elements)


def render(channels):
    panels = []
    for name, edges in channels.items():
        for number, burst in enumerate(split_bursts(edges), 1):
            panels.append((name, number, burst))

    height = TOP + max(1, len(panels)) * PANEL_HEIGHT
    body = [
        '<?xml version="1.0" encoding="UTF-8"?>',
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{WIDTH}" '
        f'height="{height}" viewBox="0 0 {WIDTH} {height}">',
        "<style>",
        "svg { background: #101418; color: #e8edf2; font-family: sans-serif; }",
        ".title { fill: #e8edf2; font-size: 15px; font-weight: 600; }",
        ".axis { fill: #aeb8c2; font-size: 12px; }",
        ".grid { stroke: #34404a; stroke-width: 1; }",
        ".tick { stroke: #aeb8c2; stroke-width: 1; }",
        ".signal { fill: none; stroke: #45e06f; stroke-width: 3; }",
        "</style>",
        '<text x="30" y="28" class="title">ESP32-S3 contact bounce</text>',
    ]
    if not panels:
        body.append(
            '<text x="30" y="70" class="axis">No WAVE records found.</text>'
        )
    for index, (name, number, edges) in enumerate(panels):
        body.append(panel(name, number, edges, TOP + index * PANEL_HEIGHT))
    body.append("</svg>")
    return "\n".join(body)


def main():
    parser = argparse.ArgumentParser(
        description="Render ESP-IDF WAVE log records as an SVG step plot."
    )
    parser.add_argument("log", type=Path)
    parser.add_argument(
        "-o", "--output", type=Path, default=Path("bounce-waveform.svg")
    )
    parser.add_argument(
        "--channel",
        help="Render only one exact WAVE channel name.",
    )
    args = parser.parse_args()

    channels = parse_log(args.log)
    if args.channel:
        channels = {
            name: edges
            for name, edges in channels.items()
            if name == args.channel
        }
    args.output.write_text(render(channels))
    print(args.output)


if __name__ == "__main__":
    main()
