from typing import List
import math
from apportionment import State


def hamilton_method(states: List[State], K: int) -> List[int]:
    """
    Hamilton (Vinton) method: Largest Remainder method.
    
    Each state receives the integer part of its quota, then remaining seats
    are allocated to states with the largest fractional parts.
    
    Args:
        states: List of State objects with name and population
        K: Total number of seats to apportion
    
    Returns:
        List of allocated seats for each state
    """
    total_population = sum(state.population for state in states)
    standard_divisor = total_population / K
    
    # Calculate quotas and initial allocations
    quotas = [state.population / standard_divisor for state in states]
    initial_seats = [int(quota) for quota in quotas]
    remainders = [quota - int(quota) for quota in quotas]
    
    # Allocate remaining seats to states with largest remainders
    total_allocated = sum(initial_seats)
    remaining_seats = K - total_allocated
    
    # Create list of (index, remainder) pairs and sort by remainder (descending)
    remainder_indices = sorted(
        enumerate(remainders),
        key=lambda x: x[1],
        reverse=True
    )
    
    # Allocate remaining seats
    final_seats = initial_seats.copy()
    for i in range(remaining_seats):
        state_idx = remainder_indices[i][0]
        final_seats[state_idx] += 1
    
    return final_seats


def jefferson_method(states: List[State], K: int) -> List[int]:
    """
    Jefferson (D'Hondt) method: Greatest Divisors method.
    
    Uses a modified divisor (smaller than standard divisor) and rounds down.
    The divisor is adjusted until the total allocated seats equals K.
    
    Args:
        states: List of State objects with name and population
        K: Total number of seats to apportion
    
    Returns:
        List of allocated seats for each state
    """
    total_population = sum(state.population for state in states)
    standard_divisor = total_population / K
    
    # Binary search for the right divisor
    # Lower bound: ensure at least K seats are allocated
    # Upper bound: standard divisor (ensures at most K seats)
    low = 0.0
    high = standard_divisor
    
    # Find divisor that gives exactly K seats
    while high - low > 1e-10:
        mid = (low + high) / 2
        seats = [int(state.population / mid) for state in states]
        total = sum(seats)
        
        if total < K:
            high = mid
        elif total > K:
            low = mid
        else:
            return seats
    
    # If exact match not found, use the divisor that gives K seats
    # Try divisors slightly below and above to find the right one
    divisor = high
    seats = [int(state.population / divisor) for state in states]
    total = sum(seats)
    
    # Adjust if needed
    if total != K:
        # Fine-tune divisor
        if total < K:
            # Need more seats, decrease divisor
            while total < K:
                divisor *= 0.999
                seats = [int(state.population / divisor) for state in states]
                total = sum(seats)
        else:
            # Need fewer seats, increase divisor
            while total > K:
                divisor *= 1.001
                seats = [int(state.population / divisor) for state in states]
                total = sum(seats)
    
    return seats


def webster_method(states: List[State], K: int) -> List[int]:
    """
    Webster (Sainte-Laguë) method: Major Fractions method.
    
    Uses rounding to nearest (0.5 as the rounding threshold) and adjusts
    the divisor until the total allocated seats equals K.
    
    Args:
        states: List of State objects with name and population
        K: Total number of seats to apportion
    
    Returns:
        List of allocated seats for each state
    """
    total_population = sum(state.population for state in states)
    standard_divisor = total_population / K
    
    # Binary search for the right divisor
    low = 0.0
    high = standard_divisor * 2  # Upper bound can be higher for rounding
    
    while high - low > 1e-10:
        mid = (low + high) / 2
        seats = [round(state.population / mid) for state in states]
        total = sum(seats)
        
        if total < K:
            high = mid
        elif total > K:
            low = mid
        else:
            return seats
    
    # If exact match not found, fine-tune
    divisor = high
    seats = [round(state.population / divisor) for state in states]
    total = sum(seats)
    
    if total != K:
        # Adjust divisor
        if total < K:
            while total < K:
                divisor *= 0.999
                seats = [round(state.population / divisor) for state in states]
                total = sum(seats)
        else:
            while total > K:
                divisor *= 1.001
                seats = [round(state.population / divisor) for state in states]
                total = sum(seats)
    
    return seats


def _huntington_hill_round(x: float) -> int:
    """
    Huntington-Hill rounding function f(x).
    
    f(x) = floor(x) if x < sqrt(floor(x) * ceil(x))
    f(x) = ceil(x) if x >= sqrt(floor(x) * ceil(x))
    
    Args:
        x: The value to round
    
    Returns:
        The rounded integer value
    """
    floor_x = math.floor(x)
    ceil_x = math.ceil(x)
    
    # Handle edge case where floor and ceil are the same (x is an integer)
    if floor_x == ceil_x:
        return int(x)
    
    # Calculate geometric mean threshold
    threshold = math.sqrt(floor_x * ceil_x)
    
    # Round based on threshold
    if x < threshold:
        return floor_x
    else:
        return ceil_x


def huntington_hill_method(states: List[State], K: int) -> List[int]:
    """
    Huntington-Hill method: Currently used by the U.S. House of Representatives.
    
    Uses a rounding function f(x) based on geometric mean threshold:
    - f(x) = floor(x) if x < sqrt(floor(x) * ceil(x))
    - f(x) = ceil(x) if x >= sqrt(floor(x) * ceil(x))
    
    Finds a divisor D such that sum of f(p_i / D) = K, where p_i is population.
    Each state is allocated k_i = f(p_i / D).
    
    Args:
        states: List of State objects with name and population
        K: Total number of seats to apportion
    
    Returns:
        List of allocated seats for each state
    """
    total_population = sum(state.population for state in states)
    standard_divisor = total_population / K
    
    # Binary search for the right divisor D
    # We need to find D such that sum of f(p_i / D) = K
    low = 0.0
    high = standard_divisor * 2  # Upper bound can be higher for rounding up
    
    while high - low > 1e-10:
        mid = (low + high) / 2
        # Calculate modified quotas and apply rounding function
        seats = [_huntington_hill_round(state.population / mid) for state in states]
        total = sum(seats)
        
        if total < K:
            high = mid
        elif total > K:
            low = mid
        else:
            return seats
    
    # If exact match not found, fine-tune
    divisor = high
    seats = [_huntington_hill_round(state.population / divisor) for state in states]
    total = sum(seats)
    
    if total != K:
        # Adjust divisor
        if total < K:
            while total < K:
                divisor *= 0.999
                seats = [_huntington_hill_round(state.population / divisor) for state in states]
                total = sum(seats)
        else:
            while total > K:
                divisor *= 1.001
                seats = [_huntington_hill_round(state.population / divisor) for state in states]
                total = sum(seats)
    
    return seats

