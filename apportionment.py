from dataclasses import dataclass
from typing import List, Callable


@dataclass
class State:
    """Represents a state with its name and population."""
    name: str
    population: int


@dataclass
class ApportionmentData:
    """Data structure containing states and total number of seats to apportion."""
    states: List[State]
    K: int  # Total number of seats to apportion


def apply_apportionment(
    data: ApportionmentData,
    apportionment_rule: Callable[[List[State], int], List[int]]
) -> List[int]:
    """
    Applies an apportionment rule to the given data structure.
    
    Args:
        data: An ApportionmentData object containing states and total seats K
        apportionment_rule: A function that takes (states, K) and returns a list
                          of allocated seats for each state
    
    Returns:
        A list of integers representing the number of seats allocated to each state,
        in the same order as the states in data.states
    """
    return apportionment_rule(data.states, data.K)

