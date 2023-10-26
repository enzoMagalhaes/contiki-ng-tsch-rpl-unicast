/***************************************************************************//**
 * @file
 * @brief factory_reset.h
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef SL_BTMESH_FACTORY_RESET_H
#define SL_BTMESH_FACTORY_RESET_H

/***************************************************************************//**
 * This function is called to initiate node reset.
 ******************************************************************************/
void sl_btmesh_initiate_node_reset(void);

/***************************************************************************//**
 * This function is called to initiate full reset.
 ******************************************************************************/
void sl_btmesh_initiate_full_reset(void);

/***************************************************************************//**
 * Handling of node reset event.
 *
 * @param[in] evt   Pointer to incoming time event.
 ******************************************************************************/
void sl_btmesh_factory_reset_on_event(sl_btmesh_msg_t *evt);

/***************************************************************************//**
 * Called during the factory reset of the node.
 * @note If no implementation is provided in the application then a default weak
 *       implementation if provided which is a no-operation. (empty function)
 *
 ******************************************************************************/
void sl_btmesh_factory_reset_on_node_reset(void);

/***************************************************************************//**
 * Called during the factory reset of the device.
 * @note If no implementation is provided in the application then a default weak
 *       implementation if provided which is a no-operation. (empty function)
 *
 ******************************************************************************/
void sl_btmesh_factory_reset_on_full_reset(void);

#endif // SL_BTMESH_FACTORY_RESET_H
