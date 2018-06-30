def df_to_str((lhs,rhs)):
        return ",".join(to_set(lhs)) + " -> " + ",".join(to_set(rhs))
