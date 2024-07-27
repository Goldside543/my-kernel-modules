#!/bin/bash
# Uses dmesg and grep to display only lines containing "Free RAM"
timeout 5s dmesg | grep "Free RAM:" || echo "No output found or command timed out."
