

def initialize_insert_data(data):
    '''
    Intialize the variable thatwant to save into database table
    '''
    code = (data['identifier'])
    timestamp = data['datetime']

    t_mean = data['data']['t_mean']
    h_mean = data['data']['h_mean']
    p_mean = data['data']['p_mean']
    l_mean = data['data']['l_mean']

    t_sd = data['data']['t_sd']
    h_sd = data['data']['h_sd']
    p_sd = data['data']['p_sd']
    l_sd = data['data']['l_sd']

    return (code, timestamp, t_mean, h_mean, p_mean, l_mean,
                           t_sd, h_sd, p_sd, l_sd)
    