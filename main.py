from apportionment import ApportionmentData, State, apply_apportionment
from apportionment_methods import (
    hamilton_method,
    jefferson_method,
    webster_method,
    huntington_hill_method
)


def get_2020_census_data():
    """
    Returns a list of State objects with 2020 US Census population data.
    Data source: 2020 United States Census
    """
    # 2020 Census population data for all 50 states
    census_data = [
        ("Alabama", 5024279),
        ("Alaska", 733391),
        ("Arizona", 7151502),
        ("Arkansas", 3011524),
        ("California", 39538223),
        ("Colorado", 5773714),
        ("Connecticut", 3605944),
        ("Delaware", 989948),
        ("Florida", 21538187),
        ("Georgia", 10711908),
        ("Hawaii", 1455271),
        ("Idaho", 1839106),
        ("Illinois", 12812508),
        ("Indiana", 6785528),
        ("Iowa", 3190369),
        ("Kansas", 2937880),
        ("Kentucky", 4505836),
        ("Louisiana", 4657757),
        ("Maine", 1362359),
        ("Maryland", 6177224),
        ("Massachusetts", 7029917),
        ("Michigan", 10037334),
        ("Minnesota", 5706494),
        ("Mississippi", 2961279),
        ("Missouri", 6154913),
        ("Montana", 1084225),
        ("Nebraska", 1961504),
        ("Nevada", 3104614),
        ("New Hampshire", 1377529),
        ("New Jersey", 9288994),
        ("New Mexico", 2117522),
        ("New York", 20201249),
        ("North Carolina", 10439388),
        ("North Dakota", 779094),
        ("Ohio", 11799448),
        ("Oklahoma", 3959353),
        ("Oregon", 4237256),
        ("Pennsylvania", 13002700),
        ("Rhode Island", 1097379),
        ("South Carolina", 5118425),
        ("South Dakota", 886667),
        ("Tennessee", 6910840),
        ("Texas", 29145505),
        ("Utah", 3271616),
        ("Vermont", 643077),
        ("Virginia", 8631393),
        ("Washington", 7705281),
        ("West Virginia", 1793716),
        ("Wisconsin", 5893718),
        ("Wyoming", 576851),
    ]
    
    return [State(name=name, population=pop) for name, pop in census_data]


def print_apportionment_results(method_name: str, states: list, seats: list):
    """Print the apportionment results in a formatted table."""
    print(f"\n{method_name} Method Results:")
    print("=" * 60)
    print(f"{'State':<20} {'Population':>15} {'Seats':>8} {'Seats per Million':>18}")
    print("-" * 60)
    
    total_population = sum(state.population for state in states)
    total_seats = sum(seats)
    
    # Sort by seats (descending) for better readability
    sorted_data = sorted(
        zip(states, seats),
        key=lambda x: x[1],
        reverse=True
    )
    
    for state, seat_count in sorted_data:
        seats_per_million = (seat_count / state.population) * 1_000_000 if state.population > 0 else 0
        print(f"{state.name:<20} {state.population:>15,} {seat_count:>8} {seats_per_million:>18.2f}")
    
    print("-" * 60)
    print(f"{'TOTAL':<20} {total_population:>15,} {total_seats:>8}")
    print("=" * 60)


def main():
    """Main function to run apportionment calculations for all methods."""
    print("U.S. House of Representatives Apportionment Analysis")
    print("Based on 2020 Census Data (K = 435 seats)")
    print("=" * 60)
    
    # Get 2020 census data
    states = get_2020_census_data()
    K = 435
    
    # Create apportionment data structure
    data = ApportionmentData(states=states, K=K)
    
    # Apply each apportionment method
    methods = [
        ("Hamilton", hamilton_method),
        ("Jefferson", jefferson_method),
        ("Webster", webster_method),
        ("Huntington-Hill", huntington_hill_method),
    ]
    
    results = {}
    for method_name, method_func in methods:
        seats = apply_apportionment(data, method_func)
        results[method_name] = seats
        print_apportionment_results(method_name, states, seats)
    
    # Summary comparison
    print("\n\nSummary Comparison:")
    print("=" * 60)
    print(f"{'State':<20} {'Hamilton':>10} {'Jefferson':>10} {'Webster':>10} {'Huntington-Hill':>15}")
    print("-" * 60)
    
    # Sort by population for comparison
    sorted_states = sorted(
        enumerate(states),
        key=lambda x: x[1].population,
        reverse=True
    )
    
    for idx, state in sorted_states:
        hamilton_seats = results["Hamilton"][idx]
        jefferson_seats = results["Jefferson"][idx]
        webster_seats = results["Webster"][idx]
        huntington_hill_seats = results["Huntington-Hill"][idx]
        
        print(f"{state.name:<20} {hamilton_seats:>10} {jefferson_seats:>10} {webster_seats:>10} {huntington_hill_seats:>15}")
    
    print("=" * 60)


if __name__ == "__main__":
    main()

