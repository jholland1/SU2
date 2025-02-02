/*!
 * \file variable_structure.hpp
 * \brief Headers of the main subroutines for storing all the variables for
 *        each kind of governing equation (direct, adjoint and linearized).
 *        The subroutines and functions are in the <i>variable_structure.cpp</i> file.
 * \author F. Palacios, T. Economon
 * \version 5.0.0 "Raven"
 *
 * SU2 Lead Developers: Dr. Francisco Palacios (Francisco.D.Palacios@boeing.com).
 *                      Dr. Thomas D. Economon (economon@stanford.edu).
 *
 * SU2 Developers: Prof. Juan J. Alonso's group at Stanford University.
 *                 Prof. Piero Colonna's group at Delft University of Technology.
 *                 Prof. Nicolas R. Gauger's group at Kaiserslautern University of Technology.
 *                 Prof. Alberto Guardone's group at Polytechnic University of Milan.
 *                 Prof. Rafael Palacios' group at Imperial College London.
 *                 Prof. Edwin van der Weide's group at the University of Twente.
 *                 Prof. Vincent Terrapon's group at the University of Liege.
 *
 * Copyright (C) 2012-2017 SU2, the open-source CFD code.
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "../../Common/include/mpi_structure.hpp"

#include <cmath>
#include <iostream>
#include <cstdlib>

#include "../../Common/include/config_structure.hpp"
#include "fluid_model.hpp"


using namespace std;

/*!
 * \class CVariable
 * \brief Main class for defining the variables.
 * \author F. Palacios
 * \version 5.0.0 "Raven"
 */
class CVariable {
protected:
  
  su2double *Solution,    /*!< \brief Solution of the problem. */
  *Solution_Old;      /*!< \brief Old solution of the problem R-K. */
  bool Non_Physical;      /*!< \brief Non-physical points in the solution (force first order). */
  su2double *Solution_time_n,  /*!< \brief Solution of the problem at time n for dual-time stepping technique. */
  *Solution_time_n1;      /*!< \brief Solution of the problem at time n-1 for dual-time stepping technique. */
  su2double **Gradient;    /*!< \brief Gradient of the solution of the problem. */
  su2double *Limiter;        /*!< \brief Limiter of the solution of the problem. */
  su2double *Solution_Max;    /*!< \brief Max solution for limiter computation. */
  su2double *Solution_Min;    /*!< \brief Min solution for limiter computation. */
  su2double AuxVar;      /*!< \brief Auxiliar variable for gradient computation. */
  su2double *Grad_AuxVar;  /*!< \brief Gradient of the auxiliar variable. */
  su2double Delta_Time;  /*!< \brief Time step. */
  su2double Max_Lambda,  /*!< \brief Maximun eingenvalue. */
  Max_Lambda_Inv,    /*!< \brief Maximun inviscid eingenvalue. */
  Max_Lambda_Visc,  /*!< \brief Maximun viscous eingenvalue. */
  Lambda;        /*!< \brief Value of the eingenvalue. */
  su2double Sensor;  /*!< \brief Pressure sensor for high order central scheme. */
  su2double *Undivided_Laplacian;  /*!< \brief Undivided laplacian of the solution. */
  su2double *Res_TruncError,  /*!< \brief Truncation error for multigrid cycle. */
  *Residual_Old,    /*!< \brief Auxiliar structure for residual smoothing. */
  *Residual_Sum;    /*!< \brief Auxiliar structure for residual smoothing. */
  static unsigned short nDim;    /*!< \brief Number of dimension of the problem. */
  unsigned short nVar;    /*!< \brief Number of variables of the problem,
                           note that this variable cannnot be static, it is possible to
                           have different number of nVar in the same problem. */
  unsigned short nPrimVar, nPrimVarGrad;    /*!< \brief Number of variables of the problem,
                                             note that this variable cannnot be static, it is possible to
                                             have different number of nVar in the same problem. */
  unsigned short nSecondaryVar, nSecondaryVarGrad;    /*!< \brief Number of variables of the problem,
                                                       note that this variable cannnot be static, it is possible to
                                                       have different number of nVar in the same problem. */
  su2double beta_fiml;				/*!< \brief Field Inversion and Machine Learning - FIML - Correction Factor. */
  su2double beta_fiml_grad;
  su2double beta_fiml_train;
  su2double fd; //JRH 08082018 - SA_DDES shielding function, computed in solver_direct_turbulent.cpp SetDES_LengthScale()
public:
  
  /*!
   * \brief Constructor of the class.
   */
  CVariable(void);
  
  /*!
   * \overload
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CVariable(unsigned short val_nvar, CConfig *config);
  
  /*!
   * \overload
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CVariable(unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  virtual ~CVariable(void);
  
  /*!
   * \brief Set the value of the solution.
   * \param[in] val_solution - Solution of the problem.
   */
  void SetSolution(su2double *val_solution);
  
  /*!
   * \overload
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution - Value of the solution for the index <i>val_var</i>.
   */
  void SetSolution(unsigned short val_var, su2double val_solution);
  
  /*!
   * \brief Add the value of the solution vector to the previous solution (incremental approach).
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution - Value of the solution for the index <i>val_var</i>.
   */
  void Add_DeltaSolution(unsigned short val_var, su2double val_solution);
  
  /*!
   * \brief Set the value of the non-physical point.
   * \param[in] val_value - identification of the non-physical point.
   */
  void SetNon_Physical(bool val_value);
  
  /*!
   * \brief Get the value of the non-physical point.
   * \return Value of the Non-physical point.
   */
  su2double GetNon_Physical(void);
  
  /*!
   * \brief Get the solution.
   * \param[in] val_var - Index of the variable.
   * \return Value of the solution for the index <i>val_var</i>.
   */
  su2double GetSolution(unsigned short val_var);
  
  /*!
   * \brief Get the old solution of the problem (Runge-Kutta method)
   * \param[in] val_var - Index of the variable.
   * \return Pointer to the old solution vector.
   */
  su2double GetSolution_Old(unsigned short val_var);
  
  /*!
   * \brief Set the value of the old solution.
   * \param[in] val_solution_old - Pointer to the residual vector.
   */
  void SetSolution_Old(su2double *val_solution_old);
  
  /*!
   * \overload
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution_old - Value of the old solution for the index <i>val_var</i>.
   */
  void SetSolution_Old(unsigned short val_var, su2double val_solution_old);
  
  /*!
   * \brief Set old variables to the value of the current variables.
   */
  void Set_OldSolution(void);
  
  /*!
   * \brief Set variables to the value of the old variables.
   */
  void Set_Solution(void);
  
  /*!
   * \brief Set the variable solution at time n.
   */
  void Set_Solution_time_n(void);
  
  /*!
   * \brief Set the variable solution at time n-1.
   */
  void Set_Solution_time_n1(void);
  
  /*!
   * \brief Set the variable solution at time n.
   */
  void Set_Solution_time_n(su2double* val_sol);
  
  /*!
   * \brief Set the variable solution at time n-1.
   */
  void Set_Solution_time_n1(su2double* val_sol);
  
  /*!
   * \brief Set to zero the velocity components of the solution.
   */
  void SetVelSolutionZero(void);
  
  /*!
   * \brief Specify a vector to set the velocity components of the solution.
   * \param[in] val_vector - Pointer to the vector.
   */
  void SetVelSolutionVector(su2double *val_vector);
  
  /*!
   * \brief Set to zero velocity components of the solution.
   */
  void SetVelSolutionOldZero(void);
  
  /*!
   * \brief Specify a vector to set the velocity components of the old solution.
   * \param[in] val_vector - Pointer to the vector.
   */
  void SetVelSolutionOldVector(su2double *val_vector);
  
  /*!
   * \brief Set to zero the solution.
   */
  void SetSolutionZero(void);
  
  /*!
   * \brief Set to zero a particular solution.
   */
  void SetSolutionZero(unsigned short val_var);
  
  /*!
   * \brief Add a value to the solution.
   * \param[in] val_var - Number of the variable.
   * \param[in] val_solution - Value that we want to add to the solution.
   */
  void AddSolution(unsigned short val_var, su2double val_solution);
  
  /*!
   * \brief Add a value to the solution, clipping the values.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution - Value of the solution change.
   * \param[in] lowerlimit - Lower value.
   * \param[in] upperlimit - Upper value.
   */
  void AddClippedSolution(unsigned short val_var, su2double val_solution,
                          su2double lowerlimit, su2double upperlimit);
  
  /*!
   * \brief Update the variables using a conservative format.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution - Value of the solution change.
   * \param[in] val_density - Value of the density.
   * \param[in] val_density_old - Value of the old density.
   * \param[in] lowerlimit - Lower value.
   * \param[in] upperlimit - Upper value.
   */
  void AddConservativeSolution(unsigned short val_var, su2double val_solution,
                               su2double val_density, su2double val_density_old, su2double lowerlimit,
                               su2double upperlimit);
  
  /*!
   * \brief Get the solution of the problem.
   * \return Pointer to the solution vector.
   */
  su2double *GetSolution(void);
  
  /*!
   * \brief Get the old solution of the problem (Runge-Kutta method)
   * \return Pointer to the old solution vector.
   */
  su2double *GetSolution_Old(void);
  
  /*!
   * \brief Get the solution at time n.
   * \return Pointer to the solution (at time n) vector.
   */
  su2double *GetSolution_time_n(void);
  
  /*!
   * \brief Get the solution at time n-1.
   * \return Pointer to the solution (at time n-1) vector.
   */
  su2double *GetSolution_time_n1(void);
  
  /*!
   * \brief Set the value of the old residual.
   * \param[in] val_residual_old - Pointer to the residual vector.
   */
  void SetResidual_Old(su2double *val_residual_old);
  
  /*!
   * \brief Add a value to the summed residual vector.
   * \param[in] val_residual - Pointer to the residual vector.
   */
  void AddResidual_Sum(su2double *val_residual);
  
  /*!
   * \brief Set summed residual vector to zero value.
   */
  void SetResidualSumZero(void);
  
  /*!
   * \brief Set the velocity of the truncation error to zero.
   */
  virtual void SetVel_ResTruncError_Zero(unsigned short iSpecies);
  
  /*!
   * \brief Get the value of the summed residual.
   * \return Pointer to the summed residual.
   */
  su2double *GetResidual_Sum(void);
  
  /*!
   * \brief Get the value of the old residual.
   * \return Pointer to the old residual.
   */
  su2double *GetResidual_Old(void);
  
  /*!
   * \brief Get the value of the summed residual.
   * \param[in] val_residual - Pointer to the summed residual.
   */
  void GetResidual_Sum(su2double *val_residual);
  
  /*!
   * \brief Set auxiliar variables, we are looking for the gradient of that variable.
   * \param[in] val_auxvar - Value of the auxiliar variable.
   */
  void SetAuxVar(su2double val_auxvar);
  
  /*!
   * \brief Get the value of the auxiliary variable.
   * \return Value of the auxiliary variable.
   */
  su2double GetAuxVar(void);
  
  /*!
   * \brief Set the auxiliary variable gradient to zero value.
   */
  void SetAuxVarGradientZero(void);
  
  /*!
   * \brief Set the value of the auxiliary variable gradient.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_gradient - Value of the gradient for the index <i>val_dim</i>.
   */
  void SetAuxVarGradient(unsigned short val_dim, su2double val_gradient);
  
  /*!
   * \brief Add a value to the auxiliary variable gradient.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value of the gradient to be added for the index <i>val_dim</i>.
   */
  void AddAuxVarGradient(unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief Subtract a value to the auxiliary variable gradient.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value of the gradient to be subtracted for the index <i>val_dim</i>.
   */
  void SubtractAuxVarGradient(unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief Get the gradient of the auxiliary variable.
   * \return Value of the gradient of the auxiliary variable.
   */
  su2double *GetAuxVarGradient(void);
  
  /*!
   * \brief Get the gradient of the auxiliary variable.
   * \param[in] val_dim - Index of the dimension.
   * \return Value of the gradient of the auxiliary variable for the dimension <i>val_dim</i>.
   */
  su2double GetAuxVarGradient(unsigned short val_dim);
  
  /*!
   * \brief Add a value to the truncation error.
   * \param[in] val_truncation_error - Value that we want to add to the truncation error.
   */
  void AddRes_TruncError(su2double *val_truncation_error);
  
  /*!
   * \brief Subtract a value to the truncation error.
   * \param[in] val_truncation_error - Value that we want to subtract to the truncation error.
   */
  void SubtractRes_TruncError(su2double *val_truncation_error);
  
  /*!
   * \brief Set the truncation error to zero.
   */
  void SetRes_TruncErrorZero(void);
  
  /*!
   * \brief Set the truncation error to zero.
   */
  void SetVal_ResTruncError_Zero(unsigned short val_var);
  
  /*!
   * \brief Set the velocity of the truncation error to zero.
   */
  void SetVel_ResTruncError_Zero(void);
  
  /*!
   * \brief Set the velocity of the truncation error to zero.
   */
  void SetEnergy_ResTruncError_Zero(void);
  
  /*!
   * \brief Get the truncation error.
   * \return Pointer to the truncation error.
   */
  su2double *GetResTruncError(void);
  
  /*!
   * \brief Get the truncation error.
   * \param[in] val_trunc_error - Pointer to the truncation error.
   */
  void GetResTruncError(su2double *val_trunc_error);
  
  /*!
   * \brief Set the gradient of the solution.
   * \param[in] val_gradient - Gradient of the solution.
   */
  void SetGradient(su2double **val_gradient);
  
  /*!
   * \overload
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value of the gradient.
   */
  void SetGradient(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief Set to zero the gradient of the solution.
   */
  void SetGradientZero(void);
  
  /*!
   * \brief Add <i>val_value</i> to the solution gradient.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value to add to the solution gradient.
   */
  void AddGradient(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief Subtract <i>val_value</i> to the solution gradient.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value to subtract to the solution gradient.
   */
  void SubtractGradient(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief Get the value of the solution gradient.
   * \return Value of the gradient solution.
   */
  su2double **GetGradient(void);
  
  /*!
   * \brief Get the value of the solution gradient.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \return Value of the solution gradient.
   */
  su2double GetGradient(unsigned short val_var, unsigned short val_dim);
  
  /*!
   * \brief Set the value of the limiter.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_limiter - Value of the limiter for the index <i>val_var</i>.
   */
  void SetLimiter(unsigned short val_var, su2double val_limiter);
  
  /*!
   * \brief Set the value of the limiter.
   * \param[in] val_species - Index of the species .
   * \param[in] val_var - Index of the variable.
   * \param[in] val_limiter - Value of the limiter for the index <i>val_var</i>.
   */
  virtual void SetLimiterPrimitive(unsigned short val_species, unsigned short val_var, su2double val_limiter);
  
  /*!
   * \brief Set the value of the limiter.
   * \param[in] val_species - Index of the species .
   * \param[in] val_var - Index of the variable.
   */
  virtual su2double GetLimiterPrimitive(unsigned short val_species, unsigned short val_var);
  
  /*!
   * \brief Set the value of the max solution.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution - Value of the max solution for the index <i>val_var</i>.
   */
  void SetSolution_Max(unsigned short val_var, su2double val_solution);
  
  /*!
   * \brief Set the value of the min solution.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution - Value of the min solution for the index <i>val_var</i>.
   */
  void SetSolution_Min(unsigned short val_var, su2double val_solution);
  
  /*!
   * \brief Get the value of the slope limiter.
   * \return Pointer to the limiters vector.
   */
  su2double *GetLimiter(void);
  
  /*!
   * \brief Get the value of the slope limiter.
   * \param[in] val_var - Index of the variable.
   * \return Value of the limiter vector for the variable <i>val_var</i>.
   */
  su2double GetLimiter(unsigned short val_var);
  
  /*!
   * \brief Get the value of the min solution.
   * \param[in] val_var - Index of the variable.
   * \return Value of the min solution for the variable <i>val_var</i>.
   */
  su2double GetSolution_Max(unsigned short val_var);
  
  /*!
   * \brief Get the value of the min solution.
   * \param[in] val_var - Index of the variable.
   * \return Value of the min solution for the variable <i>val_var</i>.
   */
  su2double GetSolution_Min(unsigned short val_var);
  
  /*!
   * \brief Get the value of the preconditioner Beta.
   * \return Value of the low Mach preconditioner variable Beta
   */
  virtual su2double GetPreconditioner_Beta();
  
  /*!
   * \brief Set the value of the preconditioner Beta.
   * \param[in] val_Beta - Value of the low Mach preconditioner variable Beta
   */
  virtual void SetPreconditioner_Beta(su2double val_Beta);
  
  /*!
   * \brief Get the value of the wind gust
   * \return Value of the wind gust
   */
  virtual su2double* GetWindGust();
  
  /*!
   * \brief Set the value of the wind gust
   * \param[in] val_WindGust - Value of the wind gust
   */
  virtual void SetWindGust(su2double* val_WindGust);
  
  /*!
   * \brief Get the value of the derivatives of the wind gust
   * \return Value of the derivatives of the wind gust
   */
  virtual su2double* GetWindGustDer();
  
  /*!
   * \brief Set the value of the derivatives of the wind gust
   * \param[in] val_WindGust - Value of the derivatives of the wind gust
   */
  virtual void SetWindGustDer(su2double* val_WindGust);
  
  /*!
   * \brief Set the value of the time step.
   * \param[in] val_delta_time - Value of the time step.
   */
  void SetDelta_Time(su2double val_delta_time);
  
  /*!
   * \brief Set the value of the time step.
   * \param[in] val_delta_time - Value of the time step.
   * \param[in] iSpecies - Index of the Species .
   */
  virtual void SetDelta_Time(su2double val_delta_time, unsigned short iSpecies);
  
  /*!
   * \brief Get the value of the time step.
   * \return Value of the time step.
   */
  su2double GetDelta_Time(void);
  
  /*!
   * \brief Get the value of the time step.
   * \param[in] iSpecies - Index of the Species
   * \return Value of the time step.
   */
  virtual su2double GetDelta_Time(unsigned short iSpecies);
  
  /*!
   * \brief Set the value of the maximum eigenvalue.
   * \param[in] val_max_lambda - Value of the maximum eigenvalue.
   */
  void SetMax_Lambda(su2double val_max_lambda);
  
  /*!
   * \brief Set the value of the maximum eigenvalue for the inviscid terms of the PDE.
   * \param[in] val_max_lambda - Value of the maximum eigenvalue for the inviscid terms of the PDE.
   */
  void SetMax_Lambda_Inv(su2double val_max_lambda);
  
  /*!
   * \brief Set the value of the maximum eigenvalue for the inviscid terms of the PDE.
   * \param[in] val_max_lambda - Value of the maximum eigenvalue for the inviscid terms of the PDE.
   * \param[in] val_species - Value of the species index to set the maximum eigenvalue.
   */
  virtual void SetMax_Lambda_Inv(su2double val_max_lambda, unsigned short val_species);
  
  /*!
   * \brief Set the value of the maximum eigenvalue for the viscous terms of the PDE.
   * \param[in] val_max_lambda - Value of the maximum eigenvalue for the viscous terms of the PDE.
   */
  void SetMax_Lambda_Visc(su2double val_max_lambda);
  
  /*!
   * \brief Set the value of the maximum eigenvalue for the viscous terms of the PDE.
   * \param[in] val_max_lambda - Value of the maximum eigenvalue for the viscous terms of the PDE.
   * \param[in] val_species - Index of the species to set the maximum eigenvalue of the viscous terms.
   */
  virtual void SetMax_Lambda_Visc(su2double val_max_lambda, unsigned short val_species);
  
  /*!
   * \brief Add a value to the maximum eigenvalue.
   * \param[in] val_max_lambda - Value of the maximum eigenvalue.
   */
  void AddMax_Lambda(su2double val_max_lambda);
  
  /*!
   * \brief Add a value to the maximum eigenvalue for the inviscid terms of the PDE.
   * \param[in] val_max_lambda - Value of the maximum eigenvalue for the inviscid terms of the PDE.
   */
  void AddMax_Lambda_Inv(su2double val_max_lambda);
  
  /*!
   * \brief Add a value to the maximum eigenvalue for the viscous terms of the PDE.
   * \param[in] val_max_lambda - Value of the maximum eigenvalue for the viscous terms of the PDE.
   */
  void AddMax_Lambda_Visc(su2double val_max_lambda);
  
  /*!
   * \brief Get the value of the maximum eigenvalue.
   * \return the value of the maximum eigenvalue.
   */
  su2double GetMax_Lambda(void);
  
  /*!
   * \brief Get the value of the maximum eigenvalue for the inviscid terms of the PDE.
   * \return the value of the maximum eigenvalue for the inviscid terms of the PDE.
   */
  su2double GetMax_Lambda_Inv(void);
  
  /*!
   * \brief Get the value of the maximum eigenvalue for the viscous terms of the PDE.
   * \return the value of the maximum eigenvalue for the viscous terms of the PDE.
   */
  su2double GetMax_Lambda_Visc(void);
  
  /*!
   * \brief Set the value of the spectral radius.
   * \param[in] val_lambda - Value of the spectral radius.
   */
  void SetLambda(su2double val_lambda);
  
  /*!
   * \brief Set the value of the spectral radius.
   * \param[in] val_lambda - Value of the spectral radius.
   * \param[in] val_iSpecies -Index of species
   */
  virtual void SetLambda(su2double val_lambda, unsigned short val_iSpecies);
  
  /*!
   * \brief Add the value of the spectral radius.
   * \param[in] val_lambda - Value of the spectral radius.
   */
  void AddLambda(su2double val_lambda);
  
  /*!
   * \brief Add the value of the spectral radius.
   * \param[in] val_iSpecies -Index of species
   * \param[in] val_lambda - Value of the spectral radius.
   */
  virtual void AddLambda(su2double val_lambda, unsigned short val_iSpecies);
  
  /*!
   * \brief Get the value of the spectral radius.
   * \return Value of the spectral radius.
   */
  su2double GetLambda(void);
  
  /*!
   * \brief Get the value of the spectral radius.
   * \param[in] val_iSpecies -Index of species
   * \return Value of the spectral radius.
   */
  virtual su2double GetLambda(unsigned short val_iSpecies);
  
  /*!
   * \brief Set pressure sensor.
   * \param[in] val_sensor - Value of the pressure sensor.
   */
  void SetSensor(su2double val_sensor);
  
  /*!
   * \brief Set pressure sensor.
   * \param[in] val_sensor - Value of the pressure sensor.
   * \param[in] iSpecies - Index of the species.
   */
  virtual void SetSensor(su2double val_sensor, unsigned short iSpecies);
  
  /*!
   * \brief Get the pressure sensor.
   * \return Value of the pressure sensor.
   */
  su2double GetSensor(void);
  
  /*!
   * \brief Get the pressure sensor.
   * \param[in] iSpecies - index of species
   * \return Value of the pressure sensor.
   */
  virtual su2double GetSensor(unsigned short iSpecies);
  
  /*!
   * \brief Set the value of the undivided laplacian of the solution.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_undivided_laplacian - Value of the undivided solution for the index <i>val_var</i>.
   */
  void SetUndivided_Laplacian(unsigned short val_var, su2double val_undivided_laplacian);
  
  /*!
   * \brief Add the value of the undivided laplacian of the solution.
   * \param[in] val_und_lapl - Value of the undivided solution.
   */
  void AddUnd_Lapl(su2double *val_und_lapl);
  
  /*!
   * \brief Subtract the value of the undivided laplacian of the solution.
   * \param[in] val_und_lapl - Value of the undivided solution.
   */
  void SubtractUnd_Lapl(su2double *val_und_lapl);
  
  /*!
   * \brief Subtract the value of the undivided laplacian of the solution.
   * \param[in] val_var - Variable of the undivided laplacian.
   * \param[in] val_und_lapl - Value of the undivided solution.
   */
  void SubtractUnd_Lapl(unsigned short val_var, su2double val_und_lapl);
  
  /*!
   * \brief Set the undivided laplacian of the solution to zero.
   */
  void SetUnd_LaplZero(void);
  
  /*!
   * \brief Set a value to the undivided laplacian.
   * \param[in] val_var - Variable of the undivided laplacian.
   * \param[in] val_und_lapl - Value of the undivided laplacian.
   */
  void SetUnd_Lapl(unsigned short val_var, su2double val_und_lapl);
  
  /*!
   * \brief Get the undivided laplacian of the solution.
   * \return Pointer to the undivided laplacian vector.
   */
  su2double *GetUndivided_Laplacian(void);
  
  /*!
   * \brief Get the undivided laplacian of the solution.
   * \param[in] val_var - Variable of the undivided laplacian.
   * \return Value of the undivided laplacian vector.
   */
  su2double GetUndivided_Laplacian(unsigned short val_var);
  
  /*!
   * \brief A virtual member.
   * \return Value of the flow density.
   */
  virtual su2double GetDensity(void);
  
  /*!
   * \brief A virtual member.
   * \return Value of the flow density.
   */
  virtual su2double GetDensity(unsigned short val_iSpecies);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_Species - Index of species s.
   * \return Value of the mass fraction of species s.
   */
  virtual su2double GetMassFraction(unsigned short val_Species);
  
  /*!
   * \brief A virtual member.
   * \return Value of the flow energy.
   */
  virtual su2double GetEnergy(void);
  
  /*!
   * \brief A virtual member.
   * \return Pointer to the force projection vector.
   */
  virtual su2double *GetForceProj_Vector(void);
  
  /*!
   * \brief A virtual member.
   * \return Pointer to the objective function source.
   */
  virtual su2double *GetObjFuncSource(void);
  
  /*!
   * \brief A virtual member.
   * \return Pointer to the internal boundary vector.
   */
  virtual su2double *GetIntBoundary_Jump(void);
  
  /*!
   * \brief A virtual member.
   * \return Value of the eddy viscosity.
   */
  virtual su2double GetEddyViscosity(void);
  
  virtual su2double GetProduction(void);
  virtual su2double GetDestruction(void);
  virtual su2double GetSTildeSA(void);
  virtual su2double GetChiSA(void);
  virtual su2double GetDeltaCriterion(void);
  virtual su2double GetFwSA(void);
  virtual su2double GetRSA(void);
  virtual su2double GetStrainMagnitude(void);
  virtual su2double GetVorticityMagnitude(void);
  virtual su2double GetGammaTrans(void); //05032018
  virtual su2double GetWallDist(void); //05032018
  virtual su2double GetkSALSA(void);



  /*!
   * \brief A virtual member.
   * \return Value of the flow enthalpy.
   */
  virtual su2double GetEnthalpy(void);
  
  /*!
   * \brief A virtual member.
   * \return Value of the flow pressure.
   */
  virtual su2double GetPressure(void);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_vector - Direction of projection.
   * \return Value of the projected velocity.
   */
  virtual su2double GetProjVel(su2double *val_vector);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_vector - Direction of projection.
   * \param[in] val_species - Index of the desired species.
   * \return Value of the projected velocity.
   */
  virtual su2double GetProjVel(su2double *val_vector, unsigned short val_species);
  
  /*!
   * \brief A virtual member.
   * \return Value of the sound speed.
   */
  virtual su2double GetSoundSpeed(void);
  
  /*!
   * \brief A virtual member.
   * \return Value of the beta for the incompressible flow.
   */
  virtual su2double GetBetaInc2(void);
  
  /*!
   * \brief A virtual member.
   * \return Value of the temperature.
   */
  virtual su2double GetTemperature(void);
  
  /*!
   * \brief A virtual member.
   * \return Value of the vibrational-electronic temperature.
   */
  virtual su2double GetTemperature_ve(void);
  
  /*!
   * \brief A virtual member -- Get the mixture specific heat at constant volume (trans.-rot.).
   * \return \f$\rho C^{t-r}_{v} \f$
   */
  virtual su2double GetRhoCv_tr(void);
  
  /*!
   * \brief A virtual member -- Get the mixture specific heat at constant volume (vib.-el.).
   * \return \f$\rho C^{v-e}_{v} \f$
   */
  virtual su2double GetRhoCv_ve(void);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_dim - Index of the dimension.
   * \return Value of the velocity for the dimension <i>val_dim</i>.
   */
  virtual su2double GetVelocity(unsigned short val_dim);
  
  /*!
   * \brief A virtual member.
   * \return Norm 2 of the velocity vector.
   */
  virtual su2double GetVelocity2(void);
  
  /*!
   * \brief A virtual member.
   * \return Norm 2 of the velocity vector of Fluid val_species.
   */
  virtual su2double GetVelocity2(unsigned short val_species);
  
  /*!
   * \brief A virtual member.
   * \return The laminar viscosity of the flow.
   */
  virtual su2double GetLaminarViscosity(void);
  
  
  /*!
   * \brief A virtual member.
   * \return The laminar viscosity of the flow.
   */
  virtual su2double GetLaminarViscosity(unsigned short iSpecies);
  
  /*!
   * \brief A virtual member.
   * \return Value of the species diffusion coefficient.
   */
  virtual su2double* GetDiffusionCoeff(void);
  
  /*!
   * \brief A virtual member.
   * \return Value of the thermal conductivity (translational/rotational)
   */
  virtual su2double GetThermalConductivity(void);
  
  /*!
   * \brief A virtual member.
   * \return Value of the specific heat at constant P
   */
  virtual su2double GetSpecificHeatCp(void);
  
  /*!
   * \brief A virtual member.
   * \return Value of the thermal conductivity (vibrational)
   */
  virtual su2double GetThermalConductivity_ve(void);
  
  /*!
   * \brief A virtual member.
   * \return Sets separation intermittency
   */
  virtual void SetGammaSep(su2double gamma_sep);
  
  /*!
   * \brief A virtual member.
   * \return Sets separation intermittency
   */
  virtual void SetGammaEff(void);
  
  /*!
   * \brief A virtual member.
   * \return Returns intermittency
   */
  virtual su2double GetIntermittency();
  
  /*!
   * \brief A virtual member.
   * \return Value of the vorticity.
   */
  virtual su2double *GetVorticity(void);
  
  /*!
   * \brief A virtual member.
   * \return Value of the rate of strain magnitude.
   */
  virtual su2double GetStrainMag(void);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_ForceProj_Vector - Pointer to the force projection vector.
   */
  virtual void SetForceProj_Vector(su2double *val_ForceProj_Vector);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_SetObjFuncSource - Pointer to the objective function source.
   */
  virtual void SetObjFuncSource(su2double *val_SetObjFuncSource);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_IntBoundary_Jump - Pointer to the interior boundary jump.
   */
  virtual void SetIntBoundary_Jump(su2double *val_IntBoundary_Jump);
  
  /*!
   * \brief A virtual member.
   * \param[in] eddy_visc - Value of the eddy viscosity.
   */
  virtual void SetEddyViscosity(su2double eddy_visc);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetEnthalpy(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual bool SetPrimVar(CConfig *config);
  
  /*!
   * \brief A virtual member.
   */
  virtual bool SetPrimVar(CFluidModel *FluidModel);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetSecondaryVar(CFluidModel *FluidModel);
  
  /*!
   * \brief A virtual member.
   */
  virtual bool Cons2PrimVar(CConfig *config, su2double *U, su2double *V,
                            su2double *dPdU, su2double *dTdU,
                            su2double *dTvedU);
  /*!
   * \brief A virtual member.
   */
  virtual void Prim2ConsVar(CConfig *config, su2double *V, su2double *U);
  
  /*!
   * \brief A virtual member.
   */
  virtual bool SetPrimVar(su2double SharpEdge_Distance, bool check, CConfig *config);
  
  /*!
   * \brief A virtual member.
   */
  virtual bool SetPrimVar(su2double eddy_visc, su2double turb_ke, CConfig *config);
  
  /*!
   * \brief A virtual member.
   */
  virtual bool SetPrimVar(su2double eddy_visc, su2double turb_ke, CFluidModel *FluidModel);
  
  /*!
   * \brief A virtual member.
   */
  virtual bool SetPrimVar(su2double Density_Inf, CConfig *config);
  
  /*!
   * \brief A virtual member.
   */
  virtual bool SetPrimVar(su2double Density_Inf, su2double Viscosity_Inf, su2double eddy_visc, su2double turb_ke, CConfig *config);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double GetPrimitive(unsigned short val_var);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetPrimitive(unsigned short val_var, su2double val_prim);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetPrimitive(su2double *val_prim);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double *GetPrimitive(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double GetSecondary(unsigned short val_var);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetSecondary(unsigned short val_var, su2double val_secondary);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetSecondary(su2double *val_secondary);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetdPdrho_e(su2double dPdrho_e);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetdPde_rho(su2double dPde_rho);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetdTdrho_e(su2double dTdrho_e);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetdTde_rho(su2double dTde_rho);
  
  /*!
   * \brief A virtual member.
   */
  virtual void Setdmudrho_T(su2double dmudrho_T);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetdmudT_rho(su2double dmudT_rho);
  
  /*!
   * \brief A virtual member.
   */
  virtual void Setdktdrho_T(su2double dktdrho_T);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetdktdT_rho(su2double dktdT_rho);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double *GetSecondary(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetDensity(su2double val_density);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetPressure(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetVelocity(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetBetaInc2(su2double val_betainc2);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_phi - Value of the adjoint velocity.
   */
  virtual void SetPhi_Old(su2double *val_phi);
  
  /*!
   * \brief A virtual member.
   * \param[in] Gamma - Ratio of Specific heats
   */
  virtual bool SetPressure(su2double Gamma);
  
  /*!
   * \brief A virtual member.
   * \param[in] config
   */
  virtual bool SetPressure(CConfig *config);
  
  /*!
   * \brief A virtual member.
   */
  virtual bool SetPressure(su2double Gamma, su2double turb_ke);
  
  /*!
   * \brief Calculates vib.-el. energy per mass, \f$e^{vib-el}_s\f$, for input species (not including KE)
   */
  virtual su2double CalcEve(su2double *V, CConfig *config, unsigned short val_Species);
  
  /*!
   * \brief Calculates enthalpy per mass, \f$h_s\f$, for input species (not including KE)
   */
  virtual su2double CalcHs(su2double *V, CConfig *config, unsigned short val_Species);
  
  /*!
   * \brief Calculates enthalpy per mass, \f$Cv_s\f$, for input species (not including KE)
   */
  virtual su2double CalcCvve(su2double val_Tve, CConfig *config, unsigned short val_Species);
  
  /*!
   * \brief A virtual member.
   * \param[in] V
   * \param[in] config - Configuration settings
   * \param[in] dPdU
   */
  virtual void CalcdPdU(su2double *V, CConfig *config, su2double *dPdU);
  
  /*!
   * \brief Set partial derivative of temperature w.r.t. density \f$\frac{\partial P}{\partial \rho_s}\f$
   * \param[in] V
   * \param[in] config - Configuration settings
   * \param[in] dTdU
   */
  virtual void CalcdTdU(su2double *V, CConfig *config, su2double *dTdU);
  
  /*!
   * \brief Set partial derivative of temperature w.r.t. density \f$\frac{\partial P}{\partial \rho_s}\f$
   * \param[in] V
   * \param[in] config - Configuration settings
   * \param[in] dTdU
   */
  virtual void CalcdTvedU(su2double *V, CConfig *config, su2double *dTdU);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double *GetdPdU(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double *GetdTdU(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double *GetdTvedU(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual bool SetDensity(void);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_velocity - Value of the velocity.
   * \param[in] Gamma - Ratio of Specific heats
   */
  virtual void SetDeltaPressure(su2double *val_velocity, su2double Gamma);
  
  /*!
   * \brief A virtual member.
   * \param[in] Gamma - Ratio of specific heats.
   */
  virtual bool SetSoundSpeed(su2double Gamma);
  
  /*!
   * \brief A virtual member.
   * \param[in] config - Configuration parameters.
   */
  virtual bool SetSoundSpeed(CConfig *config);
  
  /*!
   * \brief A virtual member.
   */
  virtual bool SetSoundSpeed(void);
  
  /*!
   * \brief A virtual member.
   * \param[in] Gas_Constant - Value of the Gas Constant
   */
  virtual bool SetTemperature(su2double Gas_Constant);
  
  /*!
   * \brief Sets the vibrational electronic temperature of the flow.
   * \return Value of the temperature of the flow.
   */
  virtual bool SetTemperature_ve(su2double val_Tve);
  
  /*!
   * \brief A virtual member.
   * \param[in] config - Configuration parameters.
   */
  virtual bool SetTemperature(CConfig *config);
  
  /*!
   * \brief A virtual member.
   * \param[in] config - Configuration parameters.
   */
  virtual void SetPrimitive(CConfig *config);
  
  /*!
   * \brief A virtual member.
   * \param[in] config - Configuration parameters.
   * \param[in] Coord - Physical coordinates.
   */
  virtual void SetPrimitive(CConfig *config, su2double *Coord);
  
  /*!
   * \brief A virtual member.
   * \param[in] Temperature_Wall - Value of the Temperature at the wall
   */
  virtual void SetWallTemperature(su2double Temperature_Wall);
  
  /*!
   * \brief A virtual member.
   * \param[in] Temperature_Wall - Value of the Temperature at the wall
   */
  virtual void SetWallTemperature(su2double* Temperature_Wall);
  
  /*!
   * \brief Set the thermal coefficient.
   * \param[in] config - Configuration parameters.
   */
  virtual void SetThermalCoeff(CConfig *config);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetStress_FEM(unsigned short iVar, su2double val_stress);
  
  /*!
   * \brief A virtual member.
   */
  virtual void AddStress_FEM(unsigned short iVar, su2double val_stress);
  
  /*!
   * \brief A virtual member.
   
   */
  virtual su2double *GetStress_FEM(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetVonMises_Stress(su2double val_stress);
  
  /*!
   * \brief A virtual member.
   
   */
  virtual su2double GetVonMises_Stress(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetTraction(unsigned short iVar, unsigned short jVar, su2double val_traction);
  
  /*!
   * \brief A virtual member.
   */
  virtual void AddTraction(unsigned short iVar, unsigned short jVar, su2double val_traction);
  
  /*!
   * \brief A virtual member.
   
   */
  virtual su2double **GetTraction(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual void Add_SurfaceLoad_Res(su2double *val_surfForce);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double *Get_SurfaceLoad_Res(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double Get_SurfaceLoad_Res(unsigned short iVar);
  
  /*!
   * \brief A virtual member.
   */
  virtual void Clear_SurfaceLoad_Res(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual void Set_SurfaceLoad_Res_n(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double Get_SurfaceLoad_Res_n(unsigned short iVar);
  
  /*!
   * \brief A virtual member.
   */
  virtual void Add_BodyForces_Res(su2double *val_bodyForce);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double *Get_BodyForces_Res(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double Get_BodyForces_Res(unsigned short iVar);
  
  /*!
   * \brief A virtual member.
   */
  virtual void Clear_BodyForces_Res(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual void Set_FlowTraction(su2double *val_flowTraction);
  
  /*!
   * \brief A virtual member.
   */
  virtual void Add_FlowTraction(su2double *val_flowTraction);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double *Get_FlowTraction(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double Get_FlowTraction(unsigned short iVar);
  
  /*!
   * \brief A virtual member.
   */
  virtual void Set_FlowTraction_n(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double Get_FlowTraction_n(unsigned short iVar);
  
  /*!
   * \brief A virtual member.
   */
  virtual void Clear_FlowTraction(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetVelocity2(void);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_velocity - Pointer to the velocity.
   */
  virtual void SetVelocity_Old(su2double *val_velocity);
  
  /*!
   * \brief A virtual member.
   * \param[in] laminarViscosity
   */
  virtual void SetLaminarViscosity(su2double laminarViscosity);
  
  /*!
   * \brief A virtual member.
   * \param[in] config - Definition of the particular problem.
   */
  virtual void SetLaminarViscosity(CConfig *config);
  
  /*!
   * \brief A virtual member.
   * \param[in] thermalConductivity
   */
  virtual void SetThermalConductivity(su2double thermalConductivity);
  
  /*!
   * \brief A virtual member.
   * \param[in] config - Definition of the particular problem.
   */
  virtual void SetThermalConductivity(CConfig *config);
  
  /*!
   * \brief A virtual member.
   * \param[in] Cp - Constant pressure specific heat.
   */
  virtual void SetSpecificHeatCp(su2double Cp);
  
  /*!
   * \brief A virtual member.
   */
  virtual bool SetVorticity(bool val_limiter);
  
  /*!
   * \brief A virtual member.
   */
  virtual bool SetStrainMag(bool val_limiter);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetVelSolutionOldDVector(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetVelSolutionDVector(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetGradient_PrimitiveZero(unsigned short val_primvar);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value to add to the gradient of the primitive variables.
   */
  virtual void AddGradient_Primitive(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value to subtract to the gradient of the primitive variables.
   */
  virtual void SubtractGradient_Primitive(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \return Value of the primitive variables gradient.
   */
  virtual su2double GetGradient_Primitive(unsigned short val_var, unsigned short val_dim);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_var - Index of the variable.
   * \return Value of the primitive variables gradient.
   */
  virtual su2double GetLimiter_Primitive(unsigned short val_var);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value of the gradient.
   */
  virtual void SetGradient_Primitive(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_value - Value of the gradient.
   */
  virtual void SetLimiter_Primitive(unsigned short val_var, su2double val_value);
  
  /*!
   * \brief A virtual member.
   * \return Value of the primitive variables gradient.
   */
  virtual su2double **GetGradient_Primitive(void);
  
  /*!
   * \brief A virtual member.
   * \return Value of the primitive variables gradient.
   */
  virtual su2double *GetLimiter_Primitive(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetGradient_SecondaryZero(unsigned short val_secondaryvar);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value to add to the gradient of the Secondary variables.
   */
  virtual void AddGradient_Secondary(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value to subtract to the gradient of the Secondary variables.
   */
  virtual void SubtractGradient_Secondary(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \return Value of the Secondary variables gradient.
   */
  virtual su2double GetGradient_Secondary(unsigned short val_var, unsigned short val_dim);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_var - Index of the variable.
   * \return Value of the Secondary variables gradient.
   */
  virtual su2double GetLimiter_Secondary(unsigned short val_var);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value of the gradient.
   */
  virtual void SetGradient_Secondary(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_value - Value of the gradient.
   */
  virtual void SetLimiter_Secondary(unsigned short val_var, su2double val_value);
  
  /*!
   * \brief A virtual member.
   * \return Value of the Secondary variables gradient.
   */
  virtual su2double **GetGradient_Secondary(void);
  
  /*!
   * \brief A virtual member.
   * \return Value of the Secondary variables gradient.
   */
  virtual su2double *GetLimiter_Secondary(void);
  
  /*!
   * \brief Set the blending function for the blending of k-w and k-eps.
   * \param[in] val_viscosity - Value of the vicosity.
   * \param[in] val_density - Value of the density.
   * \param[in] val_dist - Value of the distance to the wall.
   */
  virtual void SetBlendingFunc(su2double val_viscosity, su2double val_dist, su2double val_density);
  
  /*!
   * \brief Get the first blending function of the SST model.
   */
  virtual su2double GetF1blending(void);
  
  /*!
   * \brief Get the second blending function of the SST model.
   */
  virtual su2double GetF2blending(void);
  
  /*!
   * \brief Get the value of the cross diffusion of tke and omega.
   */
  virtual su2double GetCrossDiff(void) { return 0.0; };
  
  /*!
   * \brief Get the value of the eddy viscosity.
   * \return the value of the eddy viscosity.
   */
  virtual su2double GetmuT(void);
  
  /*!
   * \brief Set the value of the eddy viscosity.
   * \param[in] val_muT
   */
  virtual void SetmuT(su2double val_muT);
  
  /*!
   * \brief Get the value of the beta FIML Correction factor.
   * \return the value of beta FIML variable.
   */
  virtual su2double GetBetaFiml(void); //JRH - Added routine to retrieve beta FIML correction factor - 04012017
  virtual su2double GetBetaFimlTrain(void); //JRH 04182018
  /*!
   * \brief Get the value of the beta FIML Correction factor.
   * \return the value of beta FIML variable gradient.
   */
  virtual su2double GetBetaFimlGrad(void); //JRH - Added routine to retrieve beta FIML correction factor - 04012017

  /*!
   * \brief Set the value of the beta_fiml correction.
   * \param[in] val_beta_fiml
   */
  virtual void SetBetaFiml(su2double val_beta_fiml); //JRH - Added routine to set the beta FIML correction factor 04242017
  virtual void SetBetaFimlTrain(su2double val_beta_fiml); //JRH 04182018
  virtual void SetDES_fd(su2double val_fd); // JRH 08092018
  virtual su2double GetDES_fd(void); //JRH 08092018
  virtual void RegisterBeta(bool input); //JRH 05022018
  virtual void SetAdjointBeta(su2double val_adjoint_beta);//JRH 05022018
  virtual su2double GetAdjointBeta(void);//JRH 05022018


  /*!
     * \brief Set the value of the beta_fiml correction gradient.
     * \param[in] val_beta_fiml_grad
     */
   virtual void SetBetaFimlGrad(su2double val_beta_fiml_grad); //JRH - Added routine to set the beta FIML correction factor 05032017

   //routines to set values for features for machine learning - JRH 02062018
   virtual void SetProduction(su2double val_Production);
   virtual void SetDestruction(su2double val_Destruction);
   virtual void SetSTildeSA(su2double val_STildeSA);
   virtual void SetChiSA(su2double val_ChiSA);
   virtual void SetDeltaCriterion(su2double val_Delta_Criterion);
   virtual void SetFwSA(su2double val_FwSA);
   virtual void SetRSA(su2double val_RSA);
   virtual void SetVorticityMagnitude(su2double val_Omega);
   virtual void SetStrainMagnitude(su2double val_StrainMag_i);
   virtual void SetWallDist(su2double val_Wall_Dist);
   virtual void SetGammaTrans(su2double val_Gamma_Trans);
   virtual void SetkSALSA(su2double val_k_SALSA);

  /*!
   * \brief Add a value to the maximum eigenvalue for the inviscid terms of the PDE.
   * \param[in] val_max_lambda - Value of the maximum eigenvalue for the inviscid terms of the PDE.
   * \param[in] iSpecies - Value of iSpecies to which the eigenvalue belongs
   */
  virtual void AddMax_Lambda_Inv(su2double val_max_lambda, unsigned short iSpecies);
  
  /*!
   * \brief Add a value to the maximum eigenvalue for the viscous terms of the PDE.
   * \param[in] val_max_lambda - Value of the maximum eigenvalue for the viscous terms of the PDE.
   * \param[in] iSpecies - Value of iSpecies to which the eigenvalue belongs
   */
  virtual void AddMax_Lambda_Visc(su2double val_max_lambda, unsigned short iSpecies);
  
  /*!
   * \brief A virtual member.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_source - Value of the harmonic balance source.
   */
  virtual void SetHarmonicBalance_Source(unsigned short val_var, su2double val_source);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double GetHarmonicBalance_Source(unsigned short val_var);
  
  /*!
   * \brief Set the Eddy Viscosity Sensitivity of the problem.
   * \param[in] val_EddyViscSens - Eddy Viscosity Sensitivity.
   * \param[in] numTotalVar - Number of variables.
   */
  virtual void SetEddyViscSens(su2double *val_EddyViscSens, unsigned short numTotalVar);
  
  /*!
   * \brief Get the Eddy Viscosity Sensitivity of the problem.
   * \return Pointer to the Eddy Viscosity Sensitivity.
   */
  virtual su2double *GetEddyViscSens(void);
  
  /*!
   * \brief A virtual member. Set the direct solution for the adjoint solver.
   * \param[in] val_solution_direct - Value of the direct solution.
   */
  virtual void SetSolution_Direct(su2double *val_solution_direct);
  
  /*!
   * \brief A virtual member. Get the direct solution for the adjoint solver.
   * \return Pointer to the direct solution vector.
   */
  virtual su2double *GetSolution_Direct(void);
  
  /*!
   * STRUCTURAL ANALYSIS: NEW VARIABLES
   */
  
  /*!
   * \brief Set the value of the old solution.
   */
  virtual void SetSolution_time_n(void);
  
  /*!
   * \brief Set the value of the old solution.
   * \param[in] val_solution_time_n - Pointer to the residual vector.
   */
  virtual void SetSolution_time_n(unsigned short val_var, su2double val_solution);
  
  /*!
   * \brief Set the value of the old solution.
   * \param[in] val_solution_old - Pointer to the residual vector.
   */
  virtual void SetSolution_time_n(su2double *val_solution_time_n);
  
  
  /*!
   * \brief Set the value of the velocity (Structural Analysis).
   * \param[in] val_solution - Solution of the problem (velocity).
   */
  virtual void SetSolution_Vel(su2double *val_solution);
  
  /*!
   * \overload
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution_vel - Value of the solution for the index <i>val_var</i>.
   */
  virtual void SetSolution_Vel(unsigned short val_var, su2double val_solution_vel);
  
  /*!
   * \brief Set the value of the velocity (Structural Analysis) at time n.
   * \param[in] val_solution_vel_time_n - Value of the old solution.
   */
  virtual void SetSolution_Vel_time_n(su2double *val_solution_vel_time_n);
  
  /*!
   * \brief Set the value of the velocity (Structural Analysis) at time n.
   */
  virtual void SetSolution_Vel_time_n(void);
  
  /*!
   * \overload
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution_vel_time_n - Value of the old solution for the index <i>val_var</i>.
   */
  virtual void SetSolution_Vel_time_n(unsigned short val_var, su2double val_solution_vel_time_n);
  
  /*!
   * \brief Get the solution at time n.
   * \param[in] val_var - Index of the variable.
   * \return Value of the solution for the index <i>val_var</i>.
   */
  virtual su2double GetSolution_time_n(unsigned short val_var);
  
  /*!
   * \brief Get the velocity (Structural Analysis).
   * \param[in] val_var - Index of the variable.
   * \return Value of the solution for the index <i>val_var</i>.
   */
  virtual su2double GetSolution_Vel(unsigned short val_var);
  
  /*!
   * \brief Get the solution of the problem.
   * \return Pointer to the solution vector.
   */
  virtual su2double *GetSolution_Vel(void);
  
  /*!
   * \brief Get the velocity of the nodes (Structural Analysis) at time n.
   * \param[in] val_var - Index of the variable.
   * \return Pointer to the old solution vector.
   */
  virtual su2double GetSolution_Vel_time_n(unsigned short val_var);
  
  /*!
   * \brief Get the solution at time n.
   * \return Pointer to the solution (at time n) vector.
   */
  virtual su2double *GetSolution_Vel_time_n(void);
  
  
  /*!
   * \brief Set the value of the acceleration (Structural Analysis).
   * \param[in] val_solution_accel - Solution of the problem (acceleration).
   */
  virtual void SetSolution_Accel(su2double *val_solution_accel);
  
  /*!
   * \overload
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution_accel - Value of the solution for the index <i>val_var</i>.
   */
  virtual void SetSolution_Accel(unsigned short val_var, su2double val_solution_accel);
  
  /*!
   * \brief Set the value of the acceleration (Structural Analysis) at time n.
   * \param[in] val_solution_accel_time_n - Pointer to the residual vector.
   */
  virtual void SetSolution_Accel_time_n(su2double *val_solution_accel_time_n);
  
  /*!
   * \brief Set the value of the acceleration (Structural Analysis) at time n.
   */
  virtual void SetSolution_Accel_time_n(void);
  
  /*!
   * \overload
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution_accel_time_n - Value of the old solution for the index <i>val_var</i>.
   */
  virtual void SetSolution_Accel_time_n(unsigned short val_var, su2double val_solution_accel_time_n);
  
  /*!
   * \brief Get the acceleration (Structural Analysis).
   * \param[in] val_var - Index of the variable.
   * \return Value of the solution for the index <i>val_var</i>.
   */
  virtual su2double GetSolution_Accel(unsigned short val_var);
  
  /*!
   * \brief Get the solution of the problem.
   * \return Pointer to the solution vector.
   */
  virtual su2double *GetSolution_Accel(void);
  
  /*!
   * \brief Get the acceleration of the nodes (Structural Analysis) at time n.
   * \param[in] val_var - Index of the variable.
   * \return Pointer to the old solution vector.
   */
  virtual su2double GetSolution_Accel_time_n(unsigned short val_var);
  
  /*!
   * \brief Get the solution at time n.
   * \return Pointer to the solution (at time n) vector.
   */
  virtual su2double *GetSolution_Accel_time_n(void);
  
  
  /*!
   * \brief  A virtual member. Set the value of the solution predictor.
   */
  virtual void SetSolution_Pred(void);
  
  /*!
   * \brief  A virtual member. Set the value of the old solution.
   * \param[in] val_solution_pred - Pointer to the residual vector.
   */
  virtual void SetSolution_Pred(su2double *val_solution_pred);
  
  /*!
   * \brief  A virtual member. Set the value of the solution predicted.
   * \param[in] val_solution_old - Pointer to the residual vector.
   */
  virtual void SetSolution_Pred(unsigned short val_var, su2double val_solution_pred);
  
  /*!
   * \brief  A virtual member. Get the value of the solution predictor.
   * \param[in] val_var - Index of the variable.
   * \return Pointer to the old solution vector.
   */
  virtual su2double GetSolution_Pred(unsigned short val_var);
  
  /*!
   * \brief  A virtual member. Get the solution at time n.
   * \return Pointer to the solution (at time n) vector.
   */
  virtual su2double *GetSolution_Pred(void);
  
  /*!
   * \brief  A virtual member. Set the value of the solution predictor.
   */
  virtual void SetSolution_Pred_Old(void);
  
  /*!
   * \brief  A virtual member. Set the value of the old solution.
   * \param[in] val_solution_pred_Old - Pointer to the residual vector.
   */
  virtual void SetSolution_Pred_Old(su2double *val_solution_pred_Old);
  
  /*!
   * \brief  A virtual member. Set the value of the old solution predicted.
   * \param[in] val_solution_pred_old - Pointer to the residual vector.
   */
  virtual void SetSolution_Pred_Old(unsigned short val_var, su2double val_solution_pred_old);
  
  /*!
   * \brief  A virtual member. Get the value of the solution predictor.
   * \param[in] val_var - Index of the variable.
   * \return Pointer to the old solution vector.
   */
  virtual su2double GetSolution_Pred_Old(unsigned short val_var);
  
  /*!
   * \brief  A virtual member. Get the solution at time n.
   * \return Pointer to the solution (at time n) vector.
   */
  virtual su2double *GetSolution_Pred_Old(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual void SetPrestretch(unsigned short iVar, su2double val_prestretch);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double *GetPrestretch(void);
  
  /*!
   * \brief A virtual member.
   */
  virtual su2double GetPrestretch(unsigned short iVar);
  
  /*!
   * \brief Register the variables in the solution array as input/output variable.
   * \param[in] input - input or output variables.
   */
  void RegisterSolution(bool input);
  
  /*!
   * \brief Register the variables in the solution_time_n array as input/output variable.
   */
  void RegisterSolution_time_n();
  
  /*!
   * \brief Register the variables in the solution_time_n1 array as input/output variable.
   */
  void RegisterSolution_time_n1();
  
  /*!
   * \brief Set the adjoint values of the solution.
   * \param[in] adj_sol - The adjoint values of the solution.
   */
  void SetAdjointSolution(su2double *adj_sol);
  
  /*!
   * \brief Get the adjoint values of the solution.
   * \param[in] adj_sol - The adjoint values of the solution.
   */
  void GetAdjointSolution(su2double *adj_sol);
  
  /*!
   * \brief Set the adjoint values of the solution at time n.
   * \param[in] adj_sol - The adjoint values of the solution.
   */
  void SetAdjointSolution_time_n(su2double *adj_sol);
  
  /*!
   * \brief Get the adjoint values of the solution at time n.
   * \param[in] adj_sol - The adjoint values of the solution.
   */
  void GetAdjointSolution_time_n(su2double *adj_sol);
  
  /*!
   * \brief Set the adjoint values of the solution at time n-1.
   * \param[in] adj_sol - The adjoint values of the solution.
   */
  void SetAdjointSolution_time_n1(su2double *adj_sol);
  
  /*!
   * \brief Get the adjoint values of the solution at time n-1.
   * \param[in] adj_sol - The adjoint values of the solution.
   */
  void GetAdjointSolution_time_n1(su2double *adj_sol);
  
  /*!
   * \brief Set the sensitivity at the node
   * \param[in] iDim - spacial component
   * \param[in] val - value of the Sensitivity
   */
  virtual void SetSensitivity(unsigned short iDim, su2double val);
  
  /*!
   * \brief Get the Sensitivity at the node
   * \param[in] iDim - spacial component
   * \return value of the Sensitivity
   */
  virtual su2double GetSensitivity(unsigned short iDim);
  
  virtual void SetDual_Time_Derivative(unsigned short iVar, su2double der);
  
  virtual void SetDual_Time_Derivative_n(unsigned short iVar, su2double der);
  
  virtual su2double GetDual_Time_Derivative(unsigned short iVar);
  
  virtual su2double GetDual_Time_Derivative_n(unsigned short iVar);
};

/*!
 * \class CBaselineVariable
 * \brief Main class for defining the variables of a baseline solution from a restart file (for output).
 * \author F. Palacios, T. Economon.
 * \version 5.0.0 "Raven"
 */
class CBaselineVariable : public CVariable {
public:
  
  /*!
   * \brief Constructor of the class.
   */
  CBaselineVariable(void);
  
  /*!
   * \overload
   * \param[in] val_solution - Pointer to the flow value (initialization value).
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CBaselineVariable(su2double *val_solution, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  virtual ~CBaselineVariable(void);
  
};

/*!
 * \class CPotentialVariable
 * \brief Main class for defining the variables of the potential solver.
 * \ingroup Potential_Flow_Equation
 * \author F. Palacios
 * \version 5.0.0 "Raven"
 */
class CPotentialVariable : public CVariable {
  su2double *Charge_Density;
public:
  
  /*!
   * \brief Constructor of the class.
   */
  CPotentialVariable(void);
  
  /*!
   * \overload
   * \param[in] val_potential - Value of the potential solution (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CPotentialVariable(su2double val_potential, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  ~CPotentialVariable(void);
  
  /*!
   * \brief A virtual member.
   */
  su2double* GetChargeDensity();
  
  /*!
   * \brief A virtual member.
   * \param[in] positive_charge - Mass density of positive charge.
   * \param[in] negative_charge - Mass density of negative charge.
   */
  void SetChargeDensity(su2double positive_charge, su2double negative_charge);
  
};

/*!
 * \class CWaveVariable
 * \brief Main class for defining the variables of the wave equation solver.
 * \ingroup Potential_Flow_Equation
 * \author F. Palacios
 * \version 5.0.0 "Raven"
 */
class CWaveVariable : public CVariable {
protected:
  su2double *Solution_Direct;  /*!< \brief Direct solution container for use in the adjoint wave solver. */
  
public:
  
  /*!
   * \brief Constructor of the class.
   */
  CWaveVariable(void);
  
  /*!
   * \overload
   * \param[in] val_wave - Values of the wave solution (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CWaveVariable(su2double *val_wave, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  ~CWaveVariable(void);
  
  /*!
   * \brief Set the direct solution for the adjoint solver.
   * \param[in] val_solution_direct - Value of the direct solution.
   */
  void SetSolution_Direct(su2double *val_solution_direct);
  
  /*!
   * \brief Get the direct solution for the adjoint solver.
   * \return Pointer to the direct solution vector.
   */
  su2double *GetSolution_Direct(void);
  
};

/*!
 * \class CHeatVariable
 * \brief Main class for defining the variables of the Heat equation solver.
 * \ingroup Potential_Flow_Equation
 * \author F. Palacios
 * \version 5.0.0 "Raven"
 */
class CHeatVariable : public CVariable {
protected:
  su2double *Solution_Direct;  /*!< \brief Direct solution container for use in the adjoint Heat solver. */
  
public:
  
  /*!
   * \brief Constructor of the class.
   */
  CHeatVariable(void);
  
  /*!
   * \overload
   * \param[in] val_Heat - Values of the Heat solution (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CHeatVariable(su2double *val_Heat, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  ~CHeatVariable(void);
  
  /*!
   * \brief Set the direct solution for the adjoint solver.
   * \param[in] val_solution_direct - Value of the direct solution.
   */
  void SetSolution_Direct(su2double *val_solution_direct);
  
  /*!
   * \brief Get the direct solution for the adjoint solver.
   * \return Pointer to the direct solution vector.
   */
  su2double *GetSolution_Direct(void);
  
};

/*!
 * \class CFEM_ElasVariable
 * \brief Main class for defining the variables of the FEM Linear Elastic structural problem.
 * \ingroup Structural Finite Element Analysis Variables
 * \author F. Palacios, R. Sanchez.
 * \version 4.0.0 "Cardinal"
 */
class CFEM_ElasVariable : public CVariable {
protected:
  
  bool dynamic_analysis;          /*!< \brief Bool which determines if the problem is dynamic. */
  bool fsi_analysis;            /*!< \brief Bool which determines if the problem is FSI. */
  
  su2double *Stress;              /*!< \brief Stress tensor. */
  su2double *FlowTraction;          /*!< \brief Traction from the fluid field. */
  su2double *FlowTraction_n;          /*!< \brief Traction from the fluid field at time n. */
  
  //  su2double *Residual_Int;          /*!< \brief Internal stress term for the calculation of the residual */
  su2double *Residual_Ext_Surf;        /*!< \brief Term of the residual due to external forces */
  su2double *Residual_Ext_Surf_n;        /*!< \brief Term of the residual due to external forces at time n */
  su2double *Residual_Ext_Body;        /*!< \brief Term of the residual due to body forces */
  
  su2double VonMises_Stress;         /*!< \brief Von Mises stress. */
  
  su2double *Solution_time_n;        /*!< \brief Displacement at the nodes at time n */
  
  su2double *Solution_Vel,          /*!< \brief Velocity of the nodes. */
  *Solution_Vel_time_n;          /*!< \brief Velocity of the nodes at time n. */
  
  su2double *Solution_Accel,          /*!< \brief Acceleration of the nodes. */
  *Solution_Accel_time_n;          /*!< \brief Acceleration of the nodes at time n. */
  
  su2double *Solution_Pred,          /*!< \brief Predictor of the solution for FSI purposes */
  *Solution_Pred_Old;            /*!< \brief Predictor of the solution at time n for FSI purposes */
  
  su2double *Prestretch;        /*!< \brief Prestretch geometry */
  
  
public:
  
  /*!
   * \brief Constructor of the class.
   */
  CFEM_ElasVariable(void);
  
  /*!
   * \overload
   * \param[in] val_fea - Values of the fea solution (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CFEM_ElasVariable(su2double *val_fea, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  ~CFEM_ElasVariable(void);
  
  /*!
   * \brief Get the value of the stress.
   * \return Value of the stress.
   */
  su2double *GetStress_FEM(void);
  
  /*!
   * \brief Set the value of the stress at the node
   * \param[in] iVar - index of the stress term
   * \param[in] val_stress - value of the stress
   */
  void SetStress_FEM(unsigned short iVar, su2double val_stress);
  
  /*!
   * \brief Add a certain value to the value of the stress at the node
   * \param[in] iVar - index of the stress term
   * \param[in] val_stress - value of the stress
   */
  void AddStress_FEM(unsigned short iVar, su2double val_stress);
  
  /*!
   * \brief Add surface load to the residual term
   */
  void Add_SurfaceLoad_Res(su2double *val_surfForce);
  
  /*!
   * \brief Get the residual term due to surface load
   */
  su2double *Get_SurfaceLoad_Res(void);
  
  /*!
   * \brief Get the residual term due to surface load
   */
  su2double Get_SurfaceLoad_Res(unsigned short iVar);
  
  /*!
   * \brief Clear the surface load residual
   */
  void Clear_SurfaceLoad_Res(void);
  
  /*!
   * \brief Store the surface load as the load for the previous time step.
   */
  void Set_SurfaceLoad_Res_n(void);
  
  /*!
   * \brief Get the surface load from the previous time step.
   */
  su2double Get_SurfaceLoad_Res_n(unsigned short iVar);
  
  /*!
   * \brief Add body forces to the residual term.
   */
  void Add_BodyForces_Res(su2double *val_bodyForce);
  
  /*!
   * \brief Clear the surface load residual
   */
  void Clear_BodyForces_Res(void);
  
  /*!
   * \brief Get the body forces.
   */
  su2double *Get_BodyForces_Res(void);
  
  /*!
   * \brief Get the body forces.
   */
  su2double Get_BodyForces_Res(unsigned short iVar);
  
  /*!
   * \brief Set the flow traction at a node on the structural side
   */
  void Set_FlowTraction(su2double *val_flowTraction);
  
  /*!
   * \brief Add a value to the flow traction at a node on the structural side
   */
  void Add_FlowTraction(su2double *val_flowTraction);
  
  /*!
   * \brief Get the residual term due to the flow traction
   */
  su2double *Get_FlowTraction(void);
  
  /*!
   * \brief Get the residual term due to the flow traction
   */
  su2double Get_FlowTraction(unsigned short iVar);
  
  /*!
   * \brief Set the value of the flow traction at the previous time step.
   */
  void Set_FlowTraction_n(void);
  
  /*!
   * \brief Retrieve the value of the flow traction from the previous time step.
   */
  su2double Get_FlowTraction_n(unsigned short iVar);
  
  /*!
   * \brief Clear the flow traction residual
   */
  void Clear_FlowTraction(void);
  
  /*!
   * \brief Set the value of the old solution.
   * \param[in] val_solution_old - Pointer to the residual vector.
   */
  void SetSolution_time_n(void);
  
  /*!
   * \brief Set the value of the old solution.
   * \param[in] val_solution_old - Pointer to the residual vector.
   */
  void SetSolution_time_n(su2double *val_solution_time_n);
  
  /*!
   * \brief Set the value of the old solution.
   * \param[in] val_solution_old - Pointer to the residual vector.
   */
  void SetSolution_time_n(unsigned short val_var, su2double val_solution);
  
  /*!
   * \brief Set the value of the velocity (Structural Analysis).
   * \param[in] val_solution - Solution of the problem (velocity).
   */
  void SetSolution_Vel(su2double *val_solution_vel);
  
  /*!
   * \overload
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution - Value of the solution for the index <i>val_var</i>.
   */
  void SetSolution_Vel(unsigned short val_var, su2double val_solution_vel);
  
  /*!
   * \brief Set the value of the velocity (Structural Analysis) at time n.
   * \param[in] val_solution - Solution of the problem (acceleration).
   */
  void SetSolution_Vel_time_n(void);
  
  /*!
   * \brief Set the value of the velocity (Structural Analysis) at time n.
   * \param[in] val_solution_old - Pointer to the residual vector.
   */
  void SetSolution_Vel_time_n(su2double *val_solution_vel_time_n);
  
  /*!
   * \overload
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution_old - Value of the old solution for the index <i>val_var</i>.
   */
  void SetSolution_Vel_time_n(unsigned short val_var, su2double val_solution_vel_time_n);
  
  /*!
   * \brief Get the solution at time n.
   * \param[in] val_var - Index of the variable.
   * \return Value of the solution for the index <i>val_var</i>.
   */
  su2double GetSolution_time_n(unsigned short val_var);
  
  /*!
   * \brief Get the velocity (Structural Analysis).
   * \param[in] val_var - Index of the variable.
   * \return Value of the solution for the index <i>val_var</i>.
   */
  su2double GetSolution_Vel(unsigned short val_var);
  
  /*!
   * \brief Get the solution of the problem.
   * \return Pointer to the solution vector.
   */
  su2double *GetSolution_Vel(void);
  
  /*!
   * \brief Get the velocity of the nodes (Structural Analysis) at time n.
   * \param[in] val_var - Index of the variable.
   * \return Pointer to the old solution vector.
   */
  su2double GetSolution_Vel_time_n(unsigned short val_var);
  
  /*!
   * \brief Get the solution at time n.
   * \return Pointer to the solution (at time n) vector.
   */
  su2double *GetSolution_Vel_time_n(void);
  
  /*!
   * \brief Set the value of the acceleration (Structural Analysis).
   * \param[in] val_solution - Solution of the problem (acceleration).
   */
  void SetSolution_Accel(su2double *val_solution_accel);
  
  /*!
   * \overload
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution - Value of the solution for the index <i>val_var</i>.
   */
  void SetSolution_Accel(unsigned short val_var, su2double val_solution_accel);
  
  /*!
   * \brief Set the value of the acceleration (Structural Analysis) at time n.
   * \param[in] val_solution_old - Pointer to the residual vector.
   */
  void SetSolution_Accel_time_n(su2double *val_solution_accel_time_n);
  
  /*!
   * \brief Set the value of the acceleration (Structural Analysis) at time n.
   * \param[in] val_solution - Solution of the problem (acceleration).
   */
  void SetSolution_Accel_time_n(void);
  
  /*!
   * \overload
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution_old - Value of the old solution for the index <i>val_var</i>.
   */
  void SetSolution_Accel_time_n(unsigned short val_var, su2double val_solution_accel_time_n);
  
  /*!
   * \brief Get the acceleration (Structural Analysis).
   * \param[in] val_var - Index of the variable.
   * \return Value of the solution for the index <i>val_var</i>.
   */
  su2double GetSolution_Accel(unsigned short val_var);
  
  /*!
   * \brief Get the solution of the problem.
   * \return Pointer to the solution vector.
   */
  su2double *GetSolution_Accel(void);
  
  /*!
   * \brief Get the acceleration of the nodes (Structural Analysis) at time n.
   * \param[in] val_var - Index of the variable.
   * \return Pointer to the old solution vector.
   */
  su2double GetSolution_Accel_time_n(unsigned short val_var);
  
  /*!
   * \brief Get the solution at time n.
   * \return Pointer to the solution (at time n) vector.
   */
  su2double *GetSolution_Accel_time_n(void);
  
  /*!
   * \brief Set the value of the solution predictor.
   */
  void SetSolution_Pred(void);
  
  /*!
   * \brief Set the value of the old solution.
   * \param[in] val_solution_old - Pointer to the residual vector.
   */
  void SetSolution_Pred(su2double *val_solution_pred);
  
  /*!
   * \brief  Set the value of the predicted solution.
   * \param[in] val_var - Index of the variable
   * \param[in] val_solution_pred - Value of the predicted solution.
   */
  void SetSolution_Pred(unsigned short val_var, su2double val_solution_pred);
  
  /*!
   * \brief Get the value of the solution predictor.
   * \param[in] val_var - Index of the variable.
   * \return Pointer to the old solution vector.
   */
  su2double GetSolution_Pred(unsigned short val_var);
  
  /*!
   * \brief Get the solution at time n.
   * \return Pointer to the solution (at time n) vector.
   */
  su2double *GetSolution_Pred(void);
  
  /*!
   * \brief Set the value of the solution predictor.
   */
  void SetSolution_Pred_Old(void);
  
  /*!
   * \brief Set the value of the old solution.
   * \param[in] val_solution_old - Pointer to the residual vector.
   */
  void SetSolution_Pred_Old(su2double *val_solution_pred_Old);
  
  /*!
   * \brief  A virtual member. Set the value of the old solution predicted.
   * \param[in] val_var - Index of the variable
   * \param[in] val_solution_pred_old - Value of the old predicted solution.
   */
  void SetSolution_Pred_Old(unsigned short val_var, su2double val_solution_pred_old);
  
  /*!
   * \brief Get the value of the solution predictor.
   * \param[in] val_var - Index of the variable.
   * \return Pointer to the old solution vector.
   */
  su2double GetSolution_Pred_Old(unsigned short val_var);
  
  /*!
   * \brief Get the solution at time n.
   * \return Pointer to the solution (at time n) vector.
   */
  su2double *GetSolution_Pred_Old(void);
  
  /*!
   * \brief A virtual member.
   */
  void SetPrestretch(unsigned short iVar, su2double val_prestretch);
  
  /*!
   * \brief A virtual member.
   */
  su2double *GetPrestretch(void);
  
  /*!
   * \brief A virtual member.
   */
  su2double GetPrestretch(unsigned short iVar);
  
  /*!
   * \brief Set the value of the Von Mises stress.
   * \param[in] val_stress - Value of the Von Mises stress.
   */
  void SetVonMises_Stress(su2double val_stress);
  
  /*!
   * \brief Get the value of the Von Mises stress.
   * \return Value of the Von Mises stress.
   */
  su2double GetVonMises_Stress(void);
  
  
};

/*!
 * \class CFEABoundVariable
 * \brief Main class for defining the variables on the FEA boundaries for FSI applications.
 * \author R. Sanchez.
 * \version 3.2.3 "eagle"
 */
class CFEABoundVariable : public CVariable {
protected:
  su2double **Traction;  /*!< \brief Stress tensor. */
  
public:
  
  /*!
   * \brief Constructor of the class.
   */
  CFEABoundVariable(void);
  
  /*!
   * \overload
   * \param[in] val_fea - Values of the fea solution (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] val_nElBound - Number of elements in the boundary
   * \param[in] config - Definition of the particular problem.
   */
  CFEABoundVariable(unsigned short val_nDim, unsigned short val_nvar, unsigned short val_nElBound, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  ~CFEABoundVariable(void);
  
  /*!
   * \brief Set the value of the stress.
   * \param[in] iVar - index of the traction vector.
   * \param[in] jVar - index of the boundary element.
   * \param[in] val_stress - Value of the stress.
   */
  void SetTraction(unsigned short iVar, unsigned short jVar, su2double val_traction);
  
  /*!
   * \brief Add a value to the stress matrix in the element.
   * \param[in] iVar - index of the traction vector.
   * \param[in] jVar - index of the boundary element.
   * \param[in] val_stress - Value of the stress.
   */
  void AddTraction(unsigned short iVar, unsigned short jVar, su2double val_traction);
  
  /*!
   * \brief Get the value of the stress.
   * \return Value of the stress.
   */
  su2double **GetTraction(void);
  
};

/*!
 * \class CEulerVariable
 * \brief Main class for defining the variables of the compressible Euler solver.
 * \ingroup Euler_Equations
 * \author F. Palacios, T. Economon
 * \version 5.0.0 "Raven"
 */
class CEulerVariable : public CVariable {
protected:
  su2double  Velocity2;      /*!< \brief Square of the velocity vector. */
  su2double *HB_Source;     /*!< \brief harmonic balance source term. */
  su2double  Precond_Beta;  /*!< \brief Low Mach number preconditioner value, Beta. */
  su2double *WindGust;      /*! < \brief Wind gust value */
  su2double *WindGustDer;   /*! < \brief Wind gust derivatives value */
  
  /*--- Primitive variable definition ---*/
  
  su2double *Primitive;  /*!< \brief Primitive variables (T, vx, vy, vz, P, rho, h, c) in compressible flows. */
  su2double **Gradient_Primitive;  /*!< \brief Gradient of the primitive variables (T, vx, vy, vz, P, rho). */
  su2double *Limiter_Primitive;    /*!< \brief Limiter of the primitive variables (T, vx, vy, vz, P, rho). */
  
  /*--- Secondary variable definition ---*/
  
  su2double *Secondary;            /*!< \brief Primitive variables (T, vx, vy, vz, P, rho, h, c) in compressible flows. */
  su2double **Gradient_Secondary;  /*!< \brief Gradient of the primitive variables (T, vx, vy, vz, P, rho). */
  su2double *Limiter_Secondary;   /*!< \brief Limiter of the primitive variables (T, vx, vy, vz, P, rho). */
  
public:
  
  /*!
   * \brief Constructor of the class.
   */
  CEulerVariable(void);
  
  /*!
   * \overload
   * \param[in] val_density - Value of the flow density (initialization value).
   * \param[in] val_velocity - Value of the flow velocity (initialization value).
   * \param[in] val_energy - Value of the flow energy (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CEulerVariable(su2double val_density, su2double *val_velocity, su2double val_energy, unsigned short val_nDim,
                 unsigned short val_nvar, CConfig *config);
  
  /*!
   * \overload
   * \param[in] val_solution - Pointer to the flow value (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CEulerVariable(su2double *val_solution, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  virtual ~CEulerVariable(void);
  
  /*!
   * \brief Set to zero the gradient of the primitive variables.
   */
  void SetGradient_PrimitiveZero(unsigned short val_primvar);
  
  /*!
   * \brief Add <i>val_value</i> to the gradient of the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value to add to the gradient of the primitive variables.
   */
  void AddGradient_Primitive(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief Subtract <i>val_value</i> to the gradient of the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value to subtract to the gradient of the primitive variables.
   */
  void SubtractGradient_Primitive(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief Get the value of the primitive variables gradient.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \return Value of the primitive variables gradient.
   */
  su2double GetGradient_Primitive(unsigned short val_var, unsigned short val_dim);
  
  /*!
   * \brief Get the value of the primitive variables gradient.
   * \param[in] val_var - Index of the variable.
   * \return Value of the primitive variables gradient.
   */
  su2double GetLimiter_Primitive(unsigned short val_var);
  
  /*!
   * \brief Set the gradient of the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value of the gradient.
   */
  void SetGradient_Primitive(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief Set the gradient of the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_value - Value of the gradient.
   */
  void SetLimiter_Primitive(unsigned short val_var, su2double val_value);
  
  /*!
   * \brief Get the value of the primitive variables gradient.
   * \return Value of the primitive variables gradient.
   */
  su2double **GetGradient_Primitive(void);
  
  /*!
   * \brief Get the value of the primitive variables gradient.
   * \return Value of the primitive variables gradient.
   */
  su2double *GetLimiter_Primitive(void);
  
  /*!
   * \brief Set to zero the gradient of the primitive variables.
   */
  void SetGradient_SecondaryZero(unsigned short val_secondaryvar);
  
  /*!
   * \brief Add <i>val_value</i> to the gradient of the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value to add to the gradient of the primitive variables.
   */
  void AddGradient_Secondary(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief Subtract <i>val_value</i> to the gradient of the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value to subtract to the gradient of the primitive variables.
   */
  void SubtractGradient_Secondary(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief Get the value of the primitive variables gradient.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \return Value of the primitive variables gradient.
   */
  su2double GetGradient_Secondary(unsigned short val_var, unsigned short val_dim);
  
  /*!
   * \brief Get the value of the primitive variables gradient.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \return Value of the primitive variables gradient.
   */
  su2double GetLimiter_Secondary(unsigned short val_var);
  
  /*!
   * \brief Set the gradient of the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value of the gradient.
   */
  void SetGradient_Secondary(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief Set the gradient of the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value of the gradient.
   */
  void SetLimiter_Secondary(unsigned short val_var, su2double val_value);
  
  /*!
   * \brief Get the value of the primitive variables gradient.
   * \return Value of the primitive variables gradient.
   */
  su2double **GetGradient_Secondary(void);
  
  /*!
   * \brief Get the value of the primitive variables gradient.
   * \return Value of the primitive variables gradient.
   */
  su2double *GetLimiter_Secondary(void);
  
  /*!
   * \brief A virtual member.
   */
  void SetdPdrho_e(su2double dPdrho_e);
  
  /*!
   * \brief A virtual member.
   */
  void SetdPde_rho(su2double dPde_rho);
  
  /*!
   * \brief Set the value of the pressure.
   */
  bool SetPressure(su2double Gamma);
  
  /*!
   * \brief Set the value of the speed of the sound.
   * \param[in] Gamma - Value of Gamma.
   */
  bool SetSoundSpeed(su2double Gamma);
  
  /*!
   * \brief Set the value of the enthalpy.
   */
  void SetEnthalpy(void);
  
  /*!
   * \brief Set all the primitive variables for compressible flows.
   */
  bool SetPrimVar(CFluidModel *FluidModel);
  
  /*!
   * \brief A virtual member.
   */
  void SetSecondaryVar(CFluidModel *FluidModel);
  
  /*!
   * \brief Get the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \return Value of the primitive variable for the index <i>val_var</i>.
   */
  su2double GetPrimitive(unsigned short val_var);
  
  /*!
   * \brief Set the value of the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_var - Index of the variable.
   * \return Set the value of the primitive variable for the index <i>val_var</i>.
   */
  void SetPrimitive(unsigned short val_var, su2double val_prim);
  
  /*!
   * \brief Set the value of the primitive variables.
   * \param[in] val_prim - Primitive variables.
   * \return Set the value of the primitive variable for the index <i>val_var</i>.
   */
  void SetPrimitive(su2double *val_prim);
  
  /*!
   * \brief Get the primitive variables of the problem.
   * \return Pointer to the primitive variable vector.
   */
  su2double *GetPrimitive(void);
  
  /*!
   * \brief Get the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \return Value of the primitive variable for the index <i>val_var</i>.
   */
  su2double GetSecondary(unsigned short val_var);
  
  /*!
   * \brief Set the value of the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_var - Index of the variable.
   * \return Set the value of the primitive variable for the index <i>val_var</i>.
   */
  void SetSecondary(unsigned short val_var, su2double val_secondary);
  
  /*!
   * \brief Set the value of the primitive variables.
   * \param[in] val_prim - Primitive variables.
   * \return Set the value of the primitive variable for the index <i>val_var</i>.
   */
  void SetSecondary(su2double *val_secondary);
  
  /*!
   * \brief Get the primitive variables of the problem.
   * \return Pointer to the primitive variable vector.
   */
  su2double *GetSecondary(void);
  
  /*!
   * \brief Set the value of the density for the incompressible flows.
   */
  bool SetDensity(void);
  
  /*!
   * \brief Set the value of the temperature.
   * \param[in] Gas_Constant - Value of Gas Constant
   */
  bool SetTemperature(su2double Gas_Constant);
  
  /*!
   * \brief Get the norm 2 of the velocity.
   * \return Norm 2 of the velocity vector.
   */
  su2double GetVelocity2(void);
  
  /*!
   * \brief Get the flow pressure.
   * \return Value of the flow pressure.
   */
  su2double GetPressure(void);
  
  /*!
   * \brief Get the speed of the sound.
   * \return Value of speed of the sound.
   */
  su2double GetSoundSpeed(void);
  
  /*!
   * \brief Get the enthalpy of the flow.
   * \return Value of the enthalpy of the flow.
   */
  su2double GetEnthalpy(void);
  
  /*!
   * \brief Get the density of the flow.
   * \return Value of the density of the flow.
   */
  su2double GetDensity(void);
  
  /*!
   * \brief Get the energy of the flow.
   * \return Value of the energy of the flow.
   */
  su2double GetEnergy(void);
  
  /*!
   * \brief Get the temperature of the flow.
   * \return Value of the temperature of the flow.
   */
  su2double GetTemperature(void);
  
  /*!
   * \brief Get the velocity of the flow.
   * \param[in] val_dim - Index of the dimension.
   * \return Value of the velocity for the dimension <i>val_dim</i>.
   */
  su2double GetVelocity(unsigned short val_dim);
  
  /*!
   * \brief Get the projected velocity in a unitary vector direction (compressible solver).
   * \param[in] val_vector - Direction of projection.
   * \return Value of the projected velocity.
   */
  su2double GetProjVel(su2double *val_vector);
  
  /*!
   * \brief Set the velocity vector from the solution.
   * \param[in] val_velocity - Pointer to the velocity.
   */
  void SetVelocity(void);
  
  /*!
   * \brief Set the velocity vector from the old solution.
   * \param[in] val_velocity - Pointer to the velocity.
   */
  void SetVelocity_Old(su2double *val_velocity);
  
  /*!
   * \brief Set the harmonic balance source term.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution - Value of the harmonic balance source term. for the index <i>val_var</i>.
   */
  void SetHarmonicBalance_Source(unsigned short val_var, su2double val_source);
  
  /*!
   * \brief Get the harmonic balance source term.
   * \param[in] val_var - Index of the variable.
   * \return Value of the harmonic balance source term for the index <i>val_var</i>.
   */
  su2double GetHarmonicBalance_Source(unsigned short val_var);
  
  /*!
   * \brief Get the value of the preconditioner Beta.
   * \return Value of the low Mach preconditioner variable Beta
   */
  su2double GetPreconditioner_Beta();
  
  /*!
   * \brief Set the value of the preconditioner Beta.
   * \param[in] Value of the low Mach preconditioner variable Beta
   */
  void SetPreconditioner_Beta(su2double val_Beta);
  
  /*!
   * \brief Get the value of the wind gust
   * \return Value of the wind gust
   */
  su2double* GetWindGust();
  
  /*!
   * \brief Set the value of the wind gust
   * \param[in] Value of the wind gust
   */
  void SetWindGust(su2double* val_WindGust);
  
  /*!
   * \brief Get the value of the derivatives of the wind gust
   * \return Value of the derivatives of the wind gust
   */
  su2double* GetWindGustDer();
  
  /*!
   * \brief Set the value of the derivatives of the wind gust
   * \param[in] Value of the derivatives of the wind gust
   */
  void SetWindGustDer(su2double* val_WindGust);

  su2double GetBetaFiml();

  void SetBetaFiml(su2double val_beta_fiml);

  su2double GetBetaFimlTrain();

  void SetBetaFimlTrain(su2double val_beta_fiml_train);

};

/*!
 * \class CIncEulerVariable
 * \brief Main class for defining the variables of the incompressible Euler solver.
 * \ingroup Euler_Equations
 * \author F. Palacios, T. Economon, T. Albring
 * \version 5.0.0 "Raven"
 */
class CIncEulerVariable : public CVariable {
protected:
  su2double Velocity2;      /*!< \brief Square of the velocity vector. */
  su2double Precond_Beta;  /*!< \brief Low Mach number preconditioner value, Beta. */
  su2double *WindGust;           /*! < \brief Wind gust value */
  su2double *WindGustDer;        /*! < \brief Wind gust derivatives value */
  
  /*--- Primitive variable definition ---*/
  
  su2double *Primitive;  /*!< \brief Primitive variables (T, vx, vy, vz, P, rho, h, c) in compressible flows. */
  su2double **Gradient_Primitive;  /*!< \brief Gradient of the primitive variables (T, vx, vy, vz, P, rho). */
  su2double *Limiter_Primitive;    /*!< \brief Limiter of the primitive variables (T, vx, vy, vz, P, rho). */
  
public:
  
  /*!
   * \brief Constructor of the class.
   */
  CIncEulerVariable(void);
  
  /*!
   * \overload
   * \param[in] val_pressure - value of the pressure.
   * \param[in] val_velocity - Value of the flow velocity (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CIncEulerVariable(su2double val_pressure, su2double *val_velocity, unsigned short val_nDim,
                    unsigned short val_nvar, CConfig *config);
  
  /*!
   * \overload
   * \param[in] val_solution - Pointer to the flow value (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CIncEulerVariable(su2double *val_solution, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  virtual ~CIncEulerVariable(void);
  
  /*!
   * \brief Set to zero the gradient of the primitive variables.
   */
  void SetGradient_PrimitiveZero(unsigned short val_primvar);
  
  /*!
   * \brief Add <i>val_value</i> to the gradient of the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value to add to the gradient of the primitive variables.
   */
  void AddGradient_Primitive(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief Subtract <i>val_value</i> to the gradient of the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value to subtract to the gradient of the primitive variables.
   */
  void SubtractGradient_Primitive(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief Get the value of the primitive variables gradient.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \return Value of the primitive variables gradient.
   */
  su2double GetGradient_Primitive(unsigned short val_var, unsigned short val_dim);
  
  /*!
   * \brief Get the value of the primitive variables gradient.
   * \param[in] val_var - Index of the variable.
   * \return Value of the primitive variables gradient.
   */
  su2double GetLimiter_Primitive(unsigned short val_var);
  
  /*!
   * \brief Set the gradient of the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_dim - Index of the dimension.
   * \param[in] val_value - Value of the gradient.
   */
  void SetGradient_Primitive(unsigned short val_var, unsigned short val_dim, su2double val_value);
  
  /*!
   * \brief Set the gradient of the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_value - Value of the gradient.
   */
  void SetLimiter_Primitive(unsigned short val_var, su2double val_value);
  
  /*!
   * \brief Get the value of the primitive variables gradient.
   * \return Value of the primitive variables gradient.
   */
  su2double **GetGradient_Primitive(void);
  
  /*!
   * \brief Get the value of the primitive variables gradient.
   * \return Value of the primitive variables gradient.
   */
  su2double *GetLimiter_Primitive(void);
  
  /*!
   * \brief Set the value of the pressure.
   */
  void SetPressure();
  
  /*!
   * \brief Get the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \return Value of the primitive variable for the index <i>val_var</i>.
   */
  su2double GetPrimitive(unsigned short val_var);
  
  /*!
   * \brief Set the value of the primitive variables.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_var - Index of the variable.
   * \return Set the value of the primitive variable for the index <i>val_var</i>.
   */
  void SetPrimitive(unsigned short val_var, su2double val_prim);
  
  /*!
   * \brief Set the value of the primitive variables.
   * \param[in] val_prim - Primitive variables.
   * \return Set the value of the primitive variable for the index <i>val_var</i>.
   */
  void SetPrimitive(su2double *val_prim);
  
  /*!
   * \brief Get the primitive variables of the problem.
   * \return Pointer to the primitive variable vector.
   */
  su2double *GetPrimitive(void);
  
  /*!
   * \brief Set the value of the density for the incompressible flows.
   */
  void SetDensity(su2double val_density);
  
  /*!
   * \brief Set the value of the density for the incompressible flows.
   */
  void SetVelocity(void);
  
  /*!
   * \brief Set the value of the beta coeffient for incompressible flows.
   */
  void SetBetaInc2(su2double val_betainc2);
  
  /*!
   * \brief Get the norm 2 of the velocity.
   * \return Norm 2 of the velocity vector.
   */
  su2double GetVelocity2(void);
  
  /*!
   * \brief Get the flow pressure.
   * \return Value of the flow pressure.
   */
  su2double GetPressure(void);
  
  /*!
   * \brief Get the value of beta squared for the incompressible flow
   * \return Value of beta squared.
   */
  su2double GetBetaInc2(void);
  
  /*!
   * \brief Get the density of the flow.
   * \return Value of the density of the flow.
   */
  su2double GetDensity(void);
  
  /*!
   * \brief Get the velocity of the flow.
   * \param[in] val_dim - Index of the dimension.
   * \return Value of the velocity for the dimension <i>val_dim</i>.
   */
  su2double GetVelocity(unsigned short val_dim);
  
  /*!
   * \brief Get the projected velocity in a unitary vector direction (compressible solver).
   * \param[in] val_vector - Direction of projection.
   * \return Value of the projected velocity.
   */
  su2double GetProjVel(su2double *val_vector);
  
  /*!
   * \brief Set the velocity vector from the old solution.
   * \param[in] val_velocity - Pointer to the velocity.
   */
  void SetVelocity_Old(su2double *val_velocity);

  /*!
   * \brief Get the value of the wind gust
   * \return Value of the wind gust
   */
  su2double* GetWindGust();
  
  /*!
   * \brief Set the value of the wind gust
   * \param[in] Value of the wind gust
   */
  void SetWindGust(su2double* val_WindGust);
  
  /*!
   * \brief Get the value of the derivatives of the wind gust
   * \return Value of the derivatives of the wind gust
   */
  su2double* GetWindGustDer();
  
  /*!
   * \brief Set the value of the derivatives of the wind gust
   * \param[in] Value of the derivatives of the wind gust
   */
  void SetWindGustDer(su2double* val_WindGust);
  
  /*!
   * \brief Set all the primitive variables for incompressible flows.
   */
  bool SetPrimVar(su2double Density_Inf, CConfig *config);
  
  su2double GetBetaFiml();

  void SetBetaFiml(su2double val_beta_fiml);

  su2double GetBetaFimlTrain();

  void SetBetaFimlTrain(su2double val_beta_fiml_train);

};

/*!
 * \class CNSVariable
 * \brief Main class for defining the variables of the compressible Navier-Stokes solver.
 * \ingroup Navier_Stokes_Equations
 * \author F. Palacios, T. Economon
 * \version 5.0.0 "Raven"
 */
class CNSVariable : public CEulerVariable {
private:
  su2double Prandtl_Lam;     /*!< \brief Laminar Prandtl number. */
  su2double Prandtl_Turb;    /*!< \brief Turbulent Prandtl number. */
  su2double Temperature_Ref; /*!< \brief Reference temperature of the fluid. */
  su2double Viscosity_Ref;   /*!< \brief Reference viscosity of the fluid. */
  su2double Viscosity_Inf;   /*!< \brief Viscosity of the fluid at the infinity. */
  su2double Vorticity[3];    /*!< \brief Vorticity of the fluid. */
  su2double StrainMag;       /*!< \brief Magnitude of rate of strain tensor. */
public:
  
  /*!
   * \brief Constructor of the class.
   */
  CNSVariable(void);
  
  /*!
   * \overload
   * \param[in] val_density - Value of the flow density (initialization value).
   * \param[in] val_velocity - Value of the flow velocity (initialization value).
   * \param[in] val_energy - Value of the flow energy (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CNSVariable(su2double val_density, su2double *val_velocity,
              su2double val_energy, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \overload
   * \param[in] val_solution - Pointer to the flow value (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CNSVariable(su2double *val_solution, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  ~CNSVariable(void);
  
  /*!
   * \brief Set the laminar viscosity.
   */
  void SetLaminarViscosity(su2double laminarViscosity);
  
  /*!
   * \brief Set the laminar viscosity.
   */
  void SetThermalConductivity(su2double thermalConductivity);
  
  /*!
   * \brief Set the specific heat Cp.
   */
  void SetSpecificHeatCp(su2double Cp);
  
  /*!
   * \brief Set the vorticity value.
   */
  bool SetVorticity(bool val_limiter);
  
  /*!
   * \brief Set the rate of strain magnitude.
   */
  bool SetStrainMag(bool val_limiter);
  
  /*!
   * \overload
   * \param[in] eddy_visc - Value of the eddy viscosity.
   */
  void SetEddyViscosity(su2double eddy_visc);
  
  /*!
   * \brief Get the laminar viscosity of the flow.
   * \return Value of the laminar viscosity of the flow.
   */
  su2double GetLaminarViscosity(void);
  
  /*!
   * \brief Get the thermal conductivity of the flow.
   * \return Value of the laminar viscosity of the flow.
   */
  su2double GetThermalConductivity(void);
  
  /*!
   * \brief Get the eddy viscosity of the flow.
   * \return The eddy viscosity of the flow.
   */
  su2double GetEddyViscosity(void);
  
  /*!
   * \brief Get the specific heat at constant P of the flow.
   * \return Value of the specific heat at constant P  of the flow.
   */
  su2double GetSpecificHeatCp(void);
  
  /*!
   * \brief Set the temperature at the wall
   */
  void SetWallTemperature(su2double temperature_wall);
  
  /*!
   * \brief Get the value of the vorticity.
   * \param[in] val_dim - Index of the dimension.
   * \return Value of the vorticity.
   */
  su2double *GetVorticity(void);
  
  /*!
   * \brief Get the value of the magnitude of rate of strain.
   * \return Value of the rate of strain magnitude.
   */
  su2double GetStrainMag(void);
  
  /*!
   * \brief Set the derivative of temperature with respect to density (at constant internal energy).
   */
  void SetdTdrho_e(su2double dTdrho_e);
  
  /*!
   * \brief Set the derivative of temperature with respect to internal energy (at constant density).
   */
  void SetdTde_rho(su2double dTde_rho);
  
  /*!
   * \brief Set the derivative of laminar viscosity with respect to density (at constant temperature).
   */
  void Setdmudrho_T(su2double dmudrho_T);
  
  /*!
   * \brief Set the derivative of laminar viscosity with respect to temperature (at constant density).
   */
  void SetdmudT_rho(su2double dmudT_rho);
  
  /*!
   * \brief Set the derivative of thermal conductivity with respect to density (at constant temperature).
   */
  void Setdktdrho_T(su2double dktdrho_T);
  
  /*!
   * \brief Set the derivative of thermal conductivity with respect to temperature (at constant density).
   */
  void SetdktdT_rho(su2double dktdT_rho);
  
  /*!
   * \brief Set all the primitive variables for compressible flows
   */
  bool SetPrimVar(su2double eddy_visc, su2double turb_ke, CFluidModel *FluidModel);
  using CVariable::SetPrimVar;
  
  /*!
   * \brief Set all the secondary variables (partial derivatives) for compressible flows
   */
  void SetSecondaryVar(CFluidModel *FluidModel);

  su2double GetBetaFiml();

  void SetBetaFiml(su2double val_beta_fiml);

  void SetBetaFimlTrain(su2double val_beta_fiml_train);
  su2double GetBetaFimlTrain();
};

/*!
 * \class CIncNSVariable
 * \brief Main class for defining the variables of the incompressible Navier-Stokes solver.
 * \ingroup Navier_Stokes_Equations
 * \author F. Palacios, T. Economon, T. Albring
 * \version 5.0.0 "Raven"
 */
class CIncNSVariable : public CIncEulerVariable {
private:
  su2double Prandtl_Lam;     /*!< \brief Laminar Prandtl number. */
  su2double Prandtl_Turb;    /*!< \brief Turbulent Prandtl number. */
  su2double Temperature_Ref; /*!< \brief Reference temperature of the fluid. */
  su2double Viscosity_Ref;   /*!< \brief Reference viscosity of the fluid. */
  su2double Viscosity_Inf;   /*!< \brief Viscosity of the fluid at the infinity. */
  su2double Vorticity[3];    /*!< \brief Vorticity of the fluid. */
  su2double StrainMag;       /*!< \brief Magnitude of rate of strain tensor. */
public:
  
  /*!
   * \brief Constructor of the class.
   */
  CIncNSVariable(void);
  
  /*!
   * \overload
   * \param[in] val_pressure - value of the pressure.
   * \param[in] val_velocity - Value of the flow velocity (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CIncNSVariable(su2double val_pressure, su2double *val_velocity, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \overload
   * \param[in] val_solution - Pointer to the flow value (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CIncNSVariable(su2double *val_solution, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  ~CIncNSVariable(void);
  
  /*!
   * \brief Set the laminar viscosity.
   */
  void SetLaminarViscosity(su2double laminarViscosity);
  
  /*!
   * \brief Set the vorticity value.
   */
  bool SetVorticity(bool val_limiter);
  
  /*!
   * \brief Set the rate of strain magnitude.
   */
  bool SetStrainMag(bool val_limiter);
  
  /*!
   * \overload
   * \param[in] eddy_visc - Value of the eddy viscosity.
   */
  void SetEddyViscosity(su2double eddy_visc);
  
  /*!
   * \brief Get the laminar viscosity of the flow.
   * \return Value of the laminar viscosity of the flow.
   */
  su2double GetLaminarViscosity(void);
  
  /*!
   * \brief Get the eddy viscosity of the flow.
   * \return The eddy viscosity of the flow.
   */
  su2double GetEddyViscosity(void);
  
  /*!
   * \brief Get the value of the vorticity.
   * \param[in] val_dim - Index of the dimension.
   * \return Value of the vorticity.
   */
  su2double *GetVorticity(void);
  
  /*!
   * \brief Get the value of the magnitude of rate of strain.
   * \return Value of the rate of strain magnitude.
   */
  su2double GetStrainMag(void);
  
  /*!
   * \brief Set all the primitive variables for incompressible flows
   */
  su2double GetBetaFiml();
  void SetBetaFiml(su2double val_beta_fiml);
  su2double GetBetaFimlTrain();
  void SetBetaFimlTrain(su2double val_beta_fiml_train);
  bool SetPrimVar(su2double Density_Inf, su2double Viscosity_Inf, su2double eddy_visc, su2double turb_ke, CConfig *config);
  using CVariable::SetPrimVar;
  
};

/*!
 * \class CTurbVariable
 * \brief Main class for defining the variables of the turbulence model.
 * \ingroup Turbulence_Model
 * \author A. Bueno.
 * \version 5.0.0 "Raven"
 */
class CTurbVariable : public CVariable {
protected:
  su2double muT;                /*!< \brief Eddy viscosity. */
  su2double *HB_Source;          /*!< \brief Harmonic Balance source term. */
  //su2double beta_fiml;				/*!< \brief Field Inversion and Machine Learning - FIML - Correction Factor. */
  //su2double beta_fiml_grad; 	/*!< \brief Variable to store gradient of beta_fiml at each node. */
  su2double beta_fiml_train; //Variable to store optimizer betas if embedding training in inversion JRH 04182018
  //JRH - Added Beta to CTurbVariable class so it is accessible to all turbulence models - 03312017
  //JRH - Added below variables to CTurbVariable class so can output for ML features 02062018
  su2double Production;
  su2double Destruction;
  su2double STildeSA;
  su2double ChiSA;
  su2double Delta_Criterion;
  su2double FwSA;
  su2double RSA;
  su2double Strain_Magnitude;
  su2double Vorticity_Magnitude;
  su2double wall_dist; //JRH 03052018
  su2double gamma_trans; //JRH 03052018
  su2double k_SALSA;


  
public:
  /*!
   * \brief Constructor of the class.
   */
  CTurbVariable(void);
  
  /*!
   * \overload
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CTurbVariable(unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \overload
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] val_iPoint - Node index required for FIML initialization - JRH 04122017
   * \param[in] config - Definition of the particular problem.
   */
  CTurbVariable(unsigned short val_nDim, unsigned short val_nvar, unsigned short val_iPoint, CConfig *config);

  /*!
   * \brief Destructor of the class.
   */
  virtual ~CTurbVariable(void);
  
  /*!
   * \brief Get the value of the eddy viscosity.
   * \return the value of the eddy viscosity.
   */
  su2double GetmuT();
  
  /*!
   * \brief Set the value of the eddy viscosity.
   * \param[in] val_muT - Value of the eddy viscosity.
   */
  void SetmuT(su2double val_muT);

  /*!
   * \brief Get the value of the beta FIML Correction factor.
   * \return the value of beta FIML variable.
   */
  su2double GetBetaFiml(); //JRH - Added routine to retrieve beta FIML correction factor - 04012017
  su2double GetBetaFimlTrain(); //JRH - Get training value of beta FIML for neural network - 04182018

  /*!
   * \brief Get the value of the beta FIML Correction factor gradient.
   * \return the value of beta FIML variable gradient.
   */
  su2double GetBetaFimlGrad(); //JRH - Added routine to retrieve beta_fiml_grad
  //routines to set values for features for machine learning - JRH 02062018
  su2double GetProduction();
  su2double GetDestruction();
  su2double GetSTildeSA();
  su2double GetChiSA();
  su2double GetDeltaCriterion();
  su2double GetFwSA();
  su2double GetRSA();
  su2double GetStrainMagnitude();
  su2double GetVorticityMagnitude();
  su2double GetGammaTrans(void);
  su2double GetWallDist(void);
  su2double GetkSALSA(void);

  /*!
   * \brief Set the value of the beta FIML Correction factor.
   * \param[in] val_beta_fiml - the value of beta FIML variable.
   */
  void SetBetaFiml(su2double val_beta_fiml); //JRH - Added routine to set beta FIML correction factor - 04242017
  void SetBetaFimlTrain(su2double val_beta_fiml_train); //JRH 04182018 - Set value of beta to train NN on

  void SetDES_fd(su2double val_fd); // JRH 08092018
  su2double GetDES_fd(void); //JRH 08092018

  virtual void RegisterBeta(bool input); //JRH 05022018
  virtual void SetAdjointBeta(su2double val_adjoint_beta);//JRH 05022018
  virtual su2double GetAdjointBeta(void);//JRH 05022018
  /*!
   * \brief Set the value of the beta FIML Correction factor gradient.
   * \param[in] val_beta_fiml_grad - the value of beta FIML variable gradient.
   */
  void SetBetaFimlGrad(su2double val_beta_fiml_grad);

  //routines to set values for features for machine learning - JRH 02062018
  void SetProduction(su2double val_production);
  void SetDestruction(su2double val_destruction);
  void SetSTildeSA(su2double val_STilde_SA);
  void SetChiSA(su2double val_Chi_SA);
  void SetDeltaCriterion(su2double val_Delta_Criterion);
  void SetFwSA(su2double valFwSA);
  void SetRSA(su2double val_r_SA);
  void SetStrainMagnitude(su2double val_StrainMag_i);
  void SetVorticityMagnitude(su2double val_Omega);
  void SetGammaTrans(su2double val_Gamma);
  void SetWallDist(su2double val_Wall_Dist);
  void SetkSALSA(su2double val_k_SALSA);
};

/*!
 * \class CTurbSAVariable
 * \brief Main class for defining the variables of the turbulence model.
 * \ingroup Turbulence_Model
 * \author A. Bueno.
 * \version 5.0.0 "Raven"
 */

class CTurbSAVariable : public CTurbVariable {
public:
  /*!
   * \brief Constructor of the class.
   */
  CTurbSAVariable(void);
  
  /*!
   * \overload
   * \param[in] val_nu_tilde - Turbulent variable value (initialization value).
   * \param[in] val_muT  - The eddy viscosity
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CTurbSAVariable(su2double val_nu_tilde, su2double val_muT, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \overload
   * \param[in] val_nu_tilde - Turbulent variable value (initialization value).
   * \param[in] val_muT  - The eddy viscosity
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] val_iPoint - Node index required for FIML initialization - JRH 04122017
   * \param[in] config - Definition of the particular problem.
   */
  CTurbSAVariable(su2double val_nu_tilde, su2double val_muT, unsigned short val_nDim, unsigned short val_nvar, unsigned short val_iPoint, CConfig *config);

  /*!
   * \brief Destructor of the class.
   */
  ~CTurbSAVariable(void);
  
  /*!
   * \brief Set the harmonic balance source term.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_source - Value of the harmonic balance source term. for the index <i>val_var</i>.
   */
  void SetHarmonicBalance_Source(unsigned short val_var, su2double val_source);
  
  /*!
   * \brief Get the harmonic balance source term.
   * \param[in] val_var - Index of the variable.
   * \return Value of the harmonic balance source term for the index <i>val_var</i>.
   */
  su2double GetHarmonicBalance_Source(unsigned short val_var);
  
};


/*!
 * \class CTurbMLVariable
 * \brief Main class for defining the variables of the turbulence model.
 * \ingroup Turbulence_Model
 * \author A. Bueno.
 * \version 5.0.0 "Raven"
 */

class CTurbMLVariable : public CTurbVariable {
public:
  /*!
   * \brief Constructor of the class.
   */
  CTurbMLVariable(void);
  
  /*!
   * \overload
   * \param[in] val_nu_tilde - Turbulent variable value (initialization value).
   * \param[in] val_muT  - The eddy viscosity
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CTurbMLVariable(su2double val_nu_tilde, su2double val_muT, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  ~CTurbMLVariable(void);
  
  /*!
   * \brief Set the harmonic balance source term.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_source - Value of the harmonic balance source term. for the index <i>val_var</i>.
   */
  void SetHarmonicBalance_Source(unsigned short val_var, su2double val_source);
  
  /*!
   * \brief Get the harmonic balance source term.
   * \param[in] val_var - Index of the variable.
   * \return Value of the harmonic balance source term for the index <i>val_var</i>.
   */
  su2double GetHarmonicBalance_Source(unsigned short val_var);
  
};

/*!
 * \class CTransLMVariable
 * \brief Main class for defining the variables of the turbulence model.
 * \ingroup Turbulence_Model
 * \author A. Bueno.
 * \version 5.0.0 "Raven"
 */

class CTransLMVariable : public CTurbVariable {
protected:
  su2double gamma_sep;
  
public:
  
  /*!
   * \brief Constructor of the class.
   */
  CTransLMVariable(void);
  
  /*!
   * \overload
   * \param[in] val_nu_tilde - Turbulent variable value (initialization value).
   * \param[in] val_intermittency
   * \param[in] val_REth
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CTransLMVariable(su2double val_nu_tilde, su2double val_intermittency, su2double val_REth, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  ~CTransLMVariable(void);
  
  /*!
   * \brief ________________.
   */
  su2double GetIntermittency(void);
  
  /*!
   * \brief ________________.
   * \param[in] gamma_sep_in
   */
  void SetGammaSep(su2double gamma_sep_in);
  
  /*!
   * \brief ________________.
   */
  void SetGammaEff(void);
  
};

/*!
 * \class CTurbSSTVariable
 * \brief Main class for defining the variables of the turbulence model.
 * \ingroup Turbulence_Model
 * \author A. Bueno.
 * \version 5.0.0 "Raven"
 */

class CTurbSSTVariable : public CTurbVariable {
protected:
  su2double sigma_om2,
  beta_star;
  su2double F1,    /*!< \brief Menter blending function for blending of k-w and k-eps. */
  F2,            /*!< \brief Menter blending function for stress limiter. */
  CDkw;           /*!< \brief Cross-diffusion. */
  
public:
  /*!
   * \brief Constructor of the class.
   */
  CTurbSSTVariable(void);
  
  /*!
   * \overload
   * \param[in] val_rho_kine - Turbulent variable value (initialization value).
   * \param[in] val_rho_omega - Turbulent variable value (initialization value).
   * \param[in] val_muT - Turbulent variable value (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] constants -
   * \param[in] config - Definition of the particular problem.
   */
  CTurbSSTVariable(su2double val_rho_kine, su2double val_rho_omega, su2double val_muT, unsigned short val_nDim, unsigned short val_nvar,
                   su2double *constants, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  ~CTurbSSTVariable(void);
  
  /*!
   * \brief Set the blending function for the blending of k-w and k-eps.
   * \param[in] val_viscosity - Value of the vicosity.
   * \param[in] val_dist - Value of the distance to the wall.
   * \param[in] val_density - Value of the density.
   */
  void SetBlendingFunc(su2double val_viscosity, su2double val_dist, su2double val_density);
  
  /*!
   * \brief Get the first blending function.
   */
  su2double GetF1blending(void);
  
  /*!
   * \brief Get the second blending function.
   */
  su2double GetF2blending(void);
  
  /*!
   * \brief Get the value of the cross diffusion of tke and omega.
   */
  su2double GetCrossDiff(void);
};


/*!
 * \class CAdjEulerVariable
 * \brief Main class for defining the variables of the adjoint Euler solver.
 * \ingroup Euler_Equations
 * \author F. Palacios, T. Economon
 * \version 5.0.0 "Raven"
 */
class CAdjEulerVariable : public CVariable {
protected:
  su2double *Psi;    /*!< \brief Vector of the adjoint variables. */
  su2double *ForceProj_Vector;  /*!< \brief Vector d. */
  su2double *ObjFuncSource;    /*!< \brief Vector containing objective function sensitivity for discrete adjoint. */
  su2double *IntBoundary_Jump;  /*!< \brief Interior boundary jump vector. */
  su2double *HB_Source;    /*!< \brief Harmonic balance source term. */
  bool incompressible;
public:
  
  /*!
   * \brief Constructor of the class.
   */
  CAdjEulerVariable(void);
  
  /*!
   * \overload
   * \param[in] val_psirho - Value of the adjoint density (initialization value).
   * \param[in] val_phi - Value of the adjoint velocity (initialization value).
   * \param[in] val_psie - Value of the adjoint energy (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CAdjEulerVariable(su2double val_psirho, su2double *val_phi, su2double val_psie, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \overload
   * \param[in] val_solution - Pointer to the adjoint value (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CAdjEulerVariable(su2double *val_solution, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  virtual ~CAdjEulerVariable(void);
  
  /*!
   * \brief Set all the primitive variables for compressible flows.
   */
  bool SetPrimVar(su2double SharpEdge_Distance, bool check, CConfig *config);
  
  /*!
   * \brief Set the value of the adjoint velocity.
   * \param[in] val_phi - Value of the adjoint velocity.
   */
  void SetPhi_Old(su2double *val_phi);
  
  /*!
   * \brief Set the value of the force projection vector.
   * \param[in] val_ForceProj_Vector - Pointer to the force projection vector.
   */
  void SetForceProj_Vector(su2double *val_ForceProj_Vector);
  
  /*!
   * \brief Set the value of the objective function source.
   * \param[in] val_SetObjFuncSource - Pointer to the objective function source.
   */
  void SetObjFuncSource(su2double *val_SetObjFuncSource);
  
  /*!
   * \brief Set the value of the interior boundary jump vector vector.
   * \param[in] val_IntBoundary_Jump - Pointer to the interior boundary jump vector.
   */
  void SetIntBoundary_Jump(su2double *val_IntBoundary_Jump);
  
  /*!
   * \brief Get the value of the force projection vector.
   * \return Pointer to the force projection vector.
   */
  su2double *GetForceProj_Vector(void);
  
  /*!
   * \brief Get the value of the objective function source.
   * \param[in] val_SetObjFuncSource - Pointer to the objective function source.
   */
  su2double *GetObjFuncSource(void);
  
  /*!
   * \brief Get the value of the force projection vector.
   * \return Pointer to the force projection vector.
   */
  su2double *GetIntBoundary_Jump(void);
  
  /*!
   * \brief Set the harmonic balance source term.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution - Value of the harmonic balance source term. for the index <i>val_var</i>.
   */
  void SetHarmonicBalance_Source(unsigned short val_var, su2double val_source);
  
  /*!
   * \brief Get the harmonic balance source term.
   * \param[in] val_var - Index of the variable.
   * \return Value of the harmonic balance source term for the index <i>val_var</i>.
   */
  su2double GetHarmonicBalance_Source(unsigned short val_var);
};

/*!
 * \class CAdjIncEulerVariable
 * \brief Main class for defining the variables of the adjoint incompressible Euler solver.
 * \ingroup Euler_Equations
 * \author F. Palacios, T. Economon, T. Albring
 * \version 5.0.0 "Raven"
 */
class CAdjIncEulerVariable : public CVariable {
protected:
  su2double *Psi;    /*!< \brief Vector of the adjoint variables. */
  su2double *ForceProj_Vector;  /*!< \brief Vector d. */
  su2double *ObjFuncSource;    /*!< \brief Vector containing objective function sensitivity for discrete adjoint. */
  su2double *IntBoundary_Jump;  /*!< \brief Interior boundary jump vector. */
  bool incompressible;
public:
  
  /*!
   * \brief Constructor of the class.
   */
  CAdjIncEulerVariable(void);
  
  /*!
   * \overload
   * \param[in] val_psirho - Value of the adjoint density (initialization value).
   * \param[in] val_phi - Value of the adjoint velocity (initialization value).
   * \param[in] val_psie - Value of the adjoint energy (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CAdjIncEulerVariable(su2double val_psirho, su2double *val_phi, su2double val_psie, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \overload
   * \param[in] val_solution - Pointer to the adjoint value (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CAdjIncEulerVariable(su2double *val_solution, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  virtual ~CAdjIncEulerVariable(void);
  
  /*!
   * \brief Set all the primitive variables for compressible flows.
   */
  bool SetPrimVar(su2double SharpEdge_Distance, bool check, CConfig *config);
  
  /*!
   * \brief Set the value of the adjoint velocity.
   * \param[in] val_phi - Value of the adjoint velocity.
   */
  void SetPhi_Old(su2double *val_phi);
  
  /*!
   * \brief Set the value of the force projection vector.
   * \param[in] val_ForceProj_Vector - Pointer to the force projection vector.
   */
  void SetForceProj_Vector(su2double *val_ForceProj_Vector);
  
  /*!
   * \brief Set the value of the objective function source.
   * \param[in] val_SetObjFuncSource - Pointer to the objective function source.
   */
  void SetObjFuncSource(su2double *val_SetObjFuncSource);
  
  /*!
   * \brief Set the value of the interior boundary jump vector vector.
   * \param[in] val_IntBoundary_Jump - Pointer to the interior boundary jump vector.
   */
  void SetIntBoundary_Jump(su2double *val_IntBoundary_Jump);
  
  /*!
   * \brief Get the value of the force projection vector.
   * \return Pointer to the force projection vector.
   */
  su2double *GetForceProj_Vector(void);
  
  /*!
   * \brief Get the value of the objective function source.
   * \param[in] val_SetObjFuncSource - Pointer to the objective function source.
   */
  su2double *GetObjFuncSource(void);
  
  /*!
   * \brief Get the value of the force projection vector.
   * \return Pointer to the force projection vector.
   */
  su2double *GetIntBoundary_Jump(void);
  
  /*!
   * \brief Set the time spectral source term.
   * \param[in] val_var - Index of the variable.
   * \param[in] val_solution - Value of the time spectral source term. for the index <i>val_var</i>.
   */
  void SetTimeSpectral_Source(unsigned short val_var, su2double val_source);
  
  /*!
   * \brief Get the time spectral source term.
   * \param[in] val_var - Index of the variable.
   * \return Value of the time spectral source term for the index <i>val_var</i>.
   */
  su2double GetTimeSpectral_Source(unsigned short val_var);
};

/*! 
 * \class CAdjNSVariable
 * \brief Main class for defining the variables of the adjoint Navier-Stokes solver.
 * \ingroup Navier_Stokes_Equations
 * \author F. Palacios
 * \version 5.0.0 "Raven"
 */
class CAdjNSVariable : public CAdjEulerVariable {  
private:
  
public:
  
  /*!
   * \brief Constructor of the class. 
   */  
  CAdjNSVariable(void);
  
  /*!
   * \overload
   * \param[in] val_psirho - Value of the adjoint density (initialization value).
   * \param[in] val_phi - Value of the adjoint velocity (initialization value).
   * \param[in] val_psie - Value of the adjoint energy (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.     
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.   
   */  
  CAdjNSVariable(su2double val_psirho, su2double *val_phi, su2double val_psie, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \overload
   * \param[in] val_solution - Pointer to the adjoint value (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.   
   */
  CAdjNSVariable(su2double *val_solution, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class. 
   */  
  ~CAdjNSVariable(void);
  
  /*!
   * \brief Set the value of the adjoint velocity.
   * \param[in] val_phi - Value of the adjoint velocity.
   */  
  void SetPhi_Old(su2double *val_phi);
  
  /*!
   * \brief Set the value of the force projection vector.
   * \param[in] val_ForceProj_Vector - Pointer to the force projection vector.
   */
  void SetForceProj_Vector(su2double *val_ForceProj_Vector);
  
  /*!
   * \brief Get the value of the force projection vector.
   * \return Pointer to the force projection vector.
   */
  su2double *GetForceProj_Vector(void);
  
  /*!
   * \brief Set the value of the force projection vector on the solution vector.
   */
  void SetVelSolutionOldDVector(void);
  
  /*!
   * \brief Set the value of the force projection vector on the old solution vector.
   */
  void SetVelSolutionDVector(void);
  
};

/*!
 * \class CAdjIncNSVariable
 * \brief Main class for defining the variables of the adjoint incompressible Navier-Stokes solver.
 * \ingroup Navier_Stokes_Equations
 * \author F. Palacios, T. Economon, T. Albring
 * \version 5.0.0 "Raven"
 */
class CAdjIncNSVariable : public CAdjIncEulerVariable {
private:
  
public:
  
  /*!
   * \brief Constructor of the class.
   */
  CAdjIncNSVariable(void);
  
  /*!
   * \overload
   * \param[in] val_psirho - Value of the adjoint density (initialization value).
   * \param[in] val_phi - Value of the adjoint velocity (initialization value).
   * \param[in] val_psie - Value of the adjoint energy (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CAdjIncNSVariable(su2double val_psirho, su2double *val_phi, su2double val_psie, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \overload
   * \param[in] val_solution - Pointer to the adjoint value (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CAdjIncNSVariable(su2double *val_solution, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class.
   */
  ~CAdjIncNSVariable(void);
  
  /*!
   * \brief Set the value of the adjoint velocity.
   * \param[in] val_phi - Value of the adjoint velocity.
   */
  void SetPhi_Old(su2double *val_phi);
  
  /*!
   * \brief Set the value of the force projection vector.
   * \param[in] val_ForceProj_Vector - Pointer to the force projection vector.
   */
  void SetForceProj_Vector(su2double *val_ForceProj_Vector);
  
  /*!
   * \brief Get the value of the force projection vector.
   * \return Pointer to the force projection vector.
   */
  su2double *GetForceProj_Vector(void);
  
  /*!
   * \brief Set the value of the force projection vector on the solution vector.
   */
  void SetVelSolutionOldDVector(void);
  
  /*!
   * \brief Set the value of the force projection vector on the old solution vector.
   */
  void SetVelSolutionDVector(void);
  
};

/*! 
 * \class CAdjTurbVariable
 * \brief Main class for defining the variables of the adjoint turbulence model.
 * \ingroup Turbulence_Model
 * \author A. Bueno.
 * \version 5.0.0 "Raven"
 */
class CAdjTurbVariable : public CVariable {
protected:
  su2double *dmuT_dUTvar;       /*!< \brief Sensitivity of eddy viscosity to mean flow and turbulence vars. */
  su2double **dRTstar_dUTvar;   /*!< \brief Sensitivity of modified turbulence residual (no boundary flux)
                                 to mean flow and turbulence vars. */
  su2double **dFT_dUTvar;   /*!< \brief Sensitivity of boundary flux
                             to mean flow and turbulence vars. */
  su2double *EddyViscSens;    /*!< \brief Eddy Viscosity Sensitivity. */
  
public:
  
  /*!
   * \brief Constructor of the class. 
   */    
  CAdjTurbVariable(void);
  
  /*!
   * \overload
   * \param[in] val_psinu_inf - Value of the adjoint turbulence variable at the infinity (initialization value).
   * \param[in] val_nDim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.   
   */    
  CAdjTurbVariable(su2double val_psinu_inf, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class. 
   */    
  ~CAdjTurbVariable(void);
  
  /*!
   * \brief Set the Eddy Viscosity Sensitivity of the problem.
   * \param[in] val_EddyViscSens - Eddy Viscosity Sensitivity.
   */
  void SetEddyViscSens(su2double *val_EddyViscSens, unsigned short numTotalVar);
  
  /*!
   * \brief Get the Eddy Viscosity Sensitivity of the problem.
   * \return Pointer to the Eddy Viscosity Sensitivity.
   */
  su2double *GetEddyViscSens(void);
};

/*! 
 * \class CTemplateVariable
 * \brief Main class for defining the variables of the potential solver.
 * \ingroup Potential_Flow_Equation
 * \author F. Palacios
 * \version 5.0.0 "Raven"
 */
class CTemplateVariable : public CVariable {
public:
  
  /*!
   * \brief Constructor of the class. 
   */
  CTemplateVariable(void);
  
  /*!
   * \overload
   * \param[in] val_potential - Value of the potential solution (initialization value).     
   * \param[in] val_nDim - Number of dimensions of the problem.     
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.   
   */  
  CTemplateVariable(su2double val_potential, unsigned short val_nDim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Destructor of the class. 
   */  
  ~CTemplateVariable(void);
};

/*!
 * \class CDiscAdjVariable
 * \brief Main class for defining the variables of the adjoint solver.
 * \ingroup Discrete_Adjoint
 * \author T. Albring.
 * \version 5.0.0 "Raven"
 */
class CDiscAdjVariable : public CVariable {
private:
  su2double* Sensitivity; /* Vector holding the derivative of target functional with respect to the coordinates at this node*/
  su2double* Solution_Direct;
  su2double* DualTime_Derivative;
  su2double* DualTime_Derivative_n;
  
public:
  /*!
   * \brief Constructor of the class.
   */
  CDiscAdjVariable(void);
  
  /*!
   * \brief Destructor of the class.
   */
  ~CDiscAdjVariable(void);
  
  /*!
   * \overload
   * \param[in] val_solution - Pointer to the adjoint value (initialization value).
   * \param[in] val_ndim - Number of dimensions of the problem.
   * \param[in] val_nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CDiscAdjVariable(su2double *val_solution, unsigned short val_ndim, unsigned short val_nvar, CConfig *config);
  
  /*!
   * \brief Set the sensitivity at the node
   * \param[in] iDim - spacial component
   * \param[in] val - value of the Sensitivity
   */
  void SetSensitivity(unsigned short iDim, su2double val);
  
  /*!
   * \brief Get the Sensitivity at the node
   * \param[in] iDim - spacial component
   * \return value of the Sensitivity
   */
  su2double GetSensitivity(unsigned short iDim);
  
  void SetDual_Time_Derivative(unsigned short iVar, su2double der);
  
  void SetDual_Time_Derivative_n(unsigned short iVar, su2double der);
  
  su2double GetDual_Time_Derivative(unsigned short iVar);
  
  su2double GetDual_Time_Derivative_n(unsigned short iVar);
  
  void SetSolution_Direct(su2double *sol);
  
  su2double* GetSolution_Direct();

  su2double GetBetaFimlGrad();

  su2double GetBetaFiml();

  void SetBetaFiml(su2double val_beta_fiml);

  void SetBetaFimlGrad(su2double val_beta_fiml_grad);

//  void SetDES_fd(su2double val_fd); // JRH 08092018
//  su2double GetDES_fd();

};


#include "variable_structure.inl"
