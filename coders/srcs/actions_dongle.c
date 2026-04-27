#include "../include/prototype.h"
#include "../include/struct.h"
#include <stdio.h>
#include <unistd.h>

int ft_check_simulation_stop(t_data *data)
{
    int status;
    
    pthread_mutex_lock(&data->sim_mutex);
    status = data->stop_simulation;
    pthread_mutex_unlock(&data->sim_mutex);
    return (status);
}

static void ft_lock_both_dongles(t_dongle *left, t_dongle *right)
{
    if (left->id < right->id)
    {
        pthread_mutex_lock(&left->mutex);
        pthread_mutex_lock(&right->mutex);
    }
    else if (left->id > right->id)
    {
        pthread_mutex_lock(&right->mutex);
        pthread_mutex_lock(&left->mutex);
    }
    else
        pthread_mutex_lock(&left->mutex);
}

static void ft_unlock_both_dongles(t_dongle *left, t_dongle *right)
{
    pthread_mutex_unlock(&left->mutex);
    if (left != right)
        pthread_mutex_unlock(&right->mutex);
}

static int ft_can_take_dongle(t_dongle *dongle, t_coder *coder)
{
    if (dongle->is_available == false)
        return (0);
    if (ft_get_time() < dongle->available_at)
        return (0);
    if (ft_is_my_turn(dongle, coder) == 0)
        return (0);
    return (1);
}

static void ft_wait_for_dongle(t_dongle *target)
{
    struct timespec wait_until;
    long time_to_wake; 
    
    time_to_wake = ft_get_time() + 2;
    wait_until.tv_sec = time_to_wake / 1000;
    wait_until.tv_nsec = (time_to_wake % 1000) * 1000000;
    
    pthread_mutex_lock(&target->mutex);
    pthread_cond_timedwait(&target->cond, &target->mutex, &wait_until);
    pthread_mutex_unlock(&target->mutex);
}

int ft_take_dongles(t_coder *coder)
{
    t_dongle *left = coder->left_dongle;
    t_dongle *right = coder->right_dongle;
    t_dongle *blocking_dongle;

    if (left == right)
    {
        pthread_mutex_lock(&left->mutex);
        ft_print_status(coder->data, coder->id, "has taken a dongle");
        pthread_mutex_unlock(&left->mutex);
        while (!ft_check_simulation_stop(coder->data))
            usleep(1000);
        return (-1);
    }

    ft_lock_both_dongles(left, right);
    ft_add_to_queue(left, coder);
    if (left != right)
        ft_add_to_queue(right, coder);
    ft_unlock_both_dongles(left, right);

    while (!ft_check_simulation_stop(coder->data))
    {
        ft_lock_both_dongles(left, right);
        
        if (ft_can_take_dongle(left, coder) && (left == right || ft_can_take_dongle(right, coder)))
        {
            left->is_available = false;
            ft_remove_from_queue(left, coder);
            ft_print_status(coder->data, coder->id, "has taken a dongle");
            
            if (left != right)
            {
                right->is_available = false;
                ft_remove_from_queue(right, coder);
                ft_print_status(coder->data, coder->id, "has taken a dongle");
            }
            ft_unlock_both_dongles(left, right);
            return (0);
        }
        
        blocking_dongle = left;
        if (ft_can_take_dongle(left, coder) && left != right && !ft_can_take_dongle(right, coder))
            blocking_dongle = right;
            
        ft_unlock_both_dongles(left, right);
        
        ft_wait_for_dongle(blocking_dongle);
    }
    return (-1);
}

static void ft_drop_one_dongle(t_coder *coder, t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->mutex);
    dongle->is_available = true;
    dongle->available_at = ft_get_time() + coder->data->dongle_cooldown;
    pthread_cond_broadcast(&dongle->cond);
    pthread_mutex_unlock(&dongle->mutex);
}

void ft_drop_dongles(t_coder *coder)
{
    ft_drop_one_dongle(coder, coder->left_dongle);
    if (coder->left_dongle == coder->right_dongle)
        return ;
    ft_drop_one_dongle(coder, coder->right_dongle);
}